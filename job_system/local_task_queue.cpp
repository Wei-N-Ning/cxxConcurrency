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

#include <autotimer.hh>

// to test the idea of local worker queue

// NOTE:
// this version has some serious flaws! (crash)
// also shows that the performance is suboptimal; with 12 workers the speedup is merely 3.5-3.6

using Task = std::function< void() >;

struct Queue
{
    enum Status
    {
        Idle,
        Running,
        Complete,
    };

    std::vector< Task > tasks{};
    std::atomic< Status > status{ Idle };
    std::thread th{};
    std::mutex mu;

    Queue() = default;
    Queue( const Queue& ) = delete;
    Queue( Queue&& ) = delete;
    Queue& operator=( const Queue& ) = delete;
    Queue& operator=( Queue&& ) = delete;

    ~Queue()
    {
        status = Complete;
        th.join();
    }

    void wait()
    {
        while ( !tasks.empty() )
        {
            std::this_thread::sleep_for( std::chrono::nanoseconds( 200 ) );
        }
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

    void enqueue( const Task& task )
    {
        if ( status == Idle )
        {
            auto doWork = [ this, &task ]() {
                std::invoke( task );
                while ( this->status == Running )
                {
                    if ( auto t = tryPopBack(); t.has_value() )
                    {
                        std::invoke( *t );
                    }
                    else
                    {
                        std::this_thread::sleep_for( std::chrono::nanoseconds( 200 ) );
                    }
                }
            };
            status = Running;
            th = std::thread( doWork );
        }
        else if ( status == Running )
        {
            std::lock_guard l{ mu };
            tasks.emplace_back( task );
        }
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
            fib( 30 + idx % 10 );
        } );
    }

    for ( auto& child : children )
    {
        child.wait();
    }
}

int main()
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
    return 0;
}
