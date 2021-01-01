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

void test_pool( int numWorker, int numWorks, int mi, int mx )
{
    std::vector< Queue > children( numWorker );

    for ( int idx = 0; idx < numWorks; ++idx )
    {
        children[ idx % numWorker ].enqueue( [ idx, mi, mx ]() {
            //
            fib( mi + idx % ( mx - mi ) );
        } );
    }

    for ( auto& child : children )
    {
        child.done();
    }
}

void bench()
{
    constexpr int numWorks = 100;
    constexpr int baseWorkload = 30;
    constexpr int maxWorkload = 42;
    AutoTimer::Builder()
        .withLabel( "compare pool with serial execution" )
        .measure(
            //
            "serial",
            []() {
                for ( int i = 0; i < numWorks; ++i )
                {
                    fib( baseWorkload + i % ( maxWorkload - baseWorkload ) );
                }
            } )
        .measure(
            //
            []() {
                std::cout << "pool 4" << '\n';
                test_pool( 4, numWorks, baseWorkload, maxWorkload );
            } )
        .measure(
            //
            []() {
                std::cout << "pool 6" << '\n';
                test_pool( 6, numWorks, baseWorkload, maxWorkload );
            } )
        .measure(
            //
            []() {
                std::cout << "pool 8" << '\n';
                test_pool( 8, numWorks, baseWorkload, maxWorkload );
            } )
        .measure(
            //
            []() {
                std::cout << "pool 12" << '\n';
                test_pool( 12, numWorks, baseWorkload, maxWorkload );
            } );
}

int main()
{
    bench();
    return 0;
}