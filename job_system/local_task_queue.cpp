//
// Created by weining on 31/12/20.
//

#include <functional>
#include <deque>
#include <thread>
#include <mutex>
#include <iostream>
#include <optional>
#include <atomic>
#include <random>
#include <utility>

#include <autotimer.hh>

// to test the idea of local worker queue

// source of inspiration:
// https://vorbrodt.blog/2019/02/27/advanced-thread-pool/
// https://www.youtube.com/watch?v=zULU6Hhp42w

// NOTE:
// this version has some serious flaws! (crash)
// also shows that the performance is suboptimal; with 12 workers the speedup is merely 3.5-3.6
// FIXED!

// Summarising this example:
// - the goal is to demonstrate the local queue mechanism with absolutely minimal amount of code
// - use the good old poison pill pattern to signal the completion (no status, no atomic flag !)
// - work is distributed in a round robin style
// - thread will spin forever (with a tiny sleep interval) till the caller give it a poison pill
// - only look into its local queue

// with a reasonably large workload, this one is 5x faster than the serial version:
// compare pool with serial execution
//    serial (pool 1):  14875184 micro (1 runs, fastest: 14875184, slowest: 14875184)
//    pool 4         :   5098634 micro (1 runs, fastest: 5098634, slowest: 5098634) speedup: 2.91748
//    pool 6         :   3459001 micro (1 runs, fastest: 3459001, slowest: 3459001) speedup: 4.30043
//    pool 8         :   3072536 micro (1 runs, fastest: 3072536, slowest: 3072536) speedup: 4.84134
//    pool 12        :   2833933 micro (1 runs, fastest: 2833933, slowest: 2833933) speedup: 5.24895
//    constexpr int numWorks = 200;
//    constexpr int baseWorkload = 20;
//    constexpr int maxWorkload = 42;

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
    std::deque< Task > tasks{};
    std::thread th{};
    std::mutex mu;

    Queue()
    {
        auto doWork = [ this ]() {
            while ( true )
            {
                if ( auto opt = this->tryPopFront(); opt.has_value() )
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

    std::optional< Task > tryPopFront()
    {
        std::lock_guard l{ mu };
        if ( tasks.empty() )
        {
            return std::nullopt;
        }
        Task t{ tasks.front() };
        tasks.pop_front();
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
    constexpr int numWorks = 200;
    constexpr int baseWorkload = 20;
    constexpr int maxWorkload = 42;
    AutoTimer::Builder()
        .withLabel( "compare pool with serial execution" )
        .measure(
            //
            "serial (pool 1)",
            []() { test_pool( 1, numWorks, baseWorkload, maxWorkload ); } )
        .measure(
            //
            "pool 4",
            []() { test_pool( 4, numWorks, baseWorkload, maxWorkload ); } )
        .measure(
            //
            "pool 6",
            []() { test_pool( 6, numWorks, baseWorkload, maxWorkload ); } )
        .measure(
            //
            "pool 8",
            []() { test_pool( 8, numWorks, baseWorkload, maxWorkload ); } )
        .measure(
            //
            "pool 12",
            []() { test_pool( 12, numWorks, baseWorkload, maxWorkload ); } );
}

int main()
{
    bench();
    return 0;
}