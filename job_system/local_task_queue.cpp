//
// Created by weining on 31/12/20.
//

#include <functional>
#include <vector>
#include <thread>
#include <mutex>
#include <iostream>
#include <optional>
#include <atomic>
#include <random>
#include <utility>

#include <autotimer.hh>

// to test the idea of local worker queue

// NOTE:
// this version has some serious flaws! (crash)
// also shows that the performance is suboptimal; with 12 workers the speedup is merely 3.5-3.6

enum class TaskStatus
{
    Normal,
    PoisonPill,
};

using Task = std::pair< TaskStatus, std::function< void() > >;

template < typename Function >
Task makeTask( Function&& f )
{
    return std::make_pair( TaskStatus::Normal, std::forward< Function >( f ) );
}

Task makePoisonPill()
{
    return std::make_pair( TaskStatus::PoisonPill, []() {} );
}

struct Queue
{
    std::vector< Task > tasks{};
    std::thread th{};
    std::mutex mu;

    Queue()
    {
        auto doWork = [ this ]() {
            while ( true )
            {
                if ( auto opt = this->tryPopBack(); opt.has_value() )
                {
                    auto& [ status, t ] = *opt;
                    if ( status == TaskStatus::PoisonPill )
                    {
                        break;
                    }
                    std::invoke( t );
                }
                else
                {
                    // avoid spinning for nothing
                    std::this_thread::sleep_for( std::chrono::nanoseconds( 200 ) );
                }
            }
        };
        th = std::thread( doWork );
    }
    Queue( const Queue& ) = delete;
    Queue( Queue&& ) = delete;
    Queue& operator=( const Queue& ) = delete;
    Queue& operator=( Queue&& ) = delete;

    ~Queue()
    {
        th.join();
    }

    void done()
    {
        std::lock_guard l{ mu };
        tasks.emplace_back( makePoisonPill() );
    }

    std::optional< Task > tryPopBack()
    {
        std::lock_guard l{ mu };
        if ( tasks.empty() )
        {
            return std::nullopt;
        }
        Task t{ tasks.back() };
        tasks.pop_back();
        return t;
    }

    void enqueue( const std::function< void() >& fun )
    {
        std::lock_guard l{ mu };
        tasks.emplace_back( makeTask( fun ) );
    }
};

int fib( int n )
{
    return ( n > 2 ) ? fib( n - 1 ) + fib( n - 2 ) : 1;
}

void test_pool( int numWorker )
{
    std::vector< Queue > children( numWorker );

    for ( int i = 0; i < 40; ++i )
    {
        children[ i % numWorker ].enqueue( [ idx = i ]() {
            //
            fib( 20 + idx % 10 );
        } );
    }

    for ( auto& child : children )
    {
        child.done();
    }
}

void bench()
{
    AutoTimer::Builder()
        .withLabel( "compare pool with serial execution" )
        .measure( "serial",
                  []() {
                      for ( int i = 0; i < 40; ++i )
                      {
                          fib( 30 + i % 10 );
                      }
                  } )
        .measure( "pool 4", []() { test_pool( 4 ); } )
        .measure( "pool 6", []() { test_pool( 6 ); } )
        .measure( "pool 8", []() { test_pool( 8 ); } )
        .measure( "pool 12", []() { test_pool( 12 ); } );
}

int main()
{
    AutoTimer::Builder()
        .withMultiplier( 50 )
        .measure( []() { test_pool( 1 ); } )
        .measure( []() { test_pool( 2 ); } )
        .measure( []() { test_pool( 3 ); } )
        .measure( []() { test_pool( 4 ); } )
        .measure( []() { test_pool( 5 ); } )
        .measure( []() { test_pool( 6 ); } )
        .measure( []() { test_pool( 7 ); } )
        .measure( []() { test_pool( 8 ); } );

    return 0;
}