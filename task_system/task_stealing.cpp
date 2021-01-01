//
// Created by weining on 1/1/21.
//

#include <functional>
#include <deque>
#include <thread>
#include <mutex>
#include <optional>
#include <random>
#include <utility>
#include <iostream>

#include <autotimer.hh>

// this example is based on local_task_queue.cpp and adds task-stealing capability
//
// on average it performs better than the non-stealing version, however:
// - if the size of the work is not evenly divided by the number of queues, the "longest spine"
//   will cause a noticeable slow down
// - as the stealing is randomized, it does not actively look for the "longest spine", hence not
//   mitigating the above situation.
//
// numWorks: 240 base: 27 max: 38
// compare serial to pool
//    serial :   5047001 micro (1 runs, fastest: 5047001, slowest: 5047001)
//    pool 12:    910267 micro (1 runs, fastest: 910267, slowest: 910267) speedup: 5.54453
// numWorks: 24 base: 38 max: 42
// compare serial to pool
//    serial :   8508607 micro (1 runs, fastest: 8508607, slowest: 8508607)
//    pool 12:   1801398 micro (1 runs, fastest: 1801398, slowest: 1801398) speedup: 4.72334
//

// THOUGHTS:
// - parameterize the work distribution strategy: randomize, round robin
// - parameterize the job stealing strategy: randomize, look next
//
// I tested that this combination gives me a slight better perf: <work-dist: random, stealing: look next>
// this way the longest spine will be addressed quicker

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

enum class StealingStrategy
{
    LookNext,
    Random,
};

enum class WorkDistStrategy
{
    RoundRobin,
    Random,
};

template < StealingStrategy stealingStrategy >
struct Queue
{
    std::deque< Task > tasks{};
    std::optional< std::thread > th{};
    std::mutex mu;
    std::vector< Queue* > siblings{};

    std::optional< Queue* > chooseSibling()
    {
        if ( siblings.empty() )
        {
            return std::nullopt;
        }
        if constexpr ( stealingStrategy == StealingStrategy::Random )
        {
            static std::mt19937 eng{ std::random_device()() };
            static std::uniform_int_distribution< size_t > dist{};
            return siblings[ dist( eng ) % siblings.size() ];
        }
        else
        {
            static size_t idx{ 0 };
            auto p = siblings[ idx ];
            idx = ( idx + 1 ) % siblings.size();
            return p;
        }
    }

    void initialize()
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
                    if ( auto optSibling = chooseSibling(); optSibling.has_value() )
                    {
                        auto pSib = *optSibling;
                        if ( auto optSiblingTask = pSib->tryPopFront();
                             optSiblingTask.has_value()
                             && optSiblingTask->first != TaskStatus::PoisonPill )
                        {
                            std::invoke( optSiblingTask->second );
                            continue;
                        }
                    }

                    // avoid spinning for nothing
                    std::this_thread::sleep_for( std::chrono::nanoseconds( 200 ) );
                }
            }
        };
        th = std::thread( doWork );
    }

    Queue() = default;
    Queue( const Queue& ) = delete;
    Queue( Queue&& ) = delete;
    Queue& operator=( const Queue& ) = delete;
    Queue& operator=( Queue&& ) = delete;

    ~Queue()
    {
        if ( th.has_value() )
        {
            th->join();
        }
    }

    void connectWithSiblings( std::vector< Queue >& qs )
    {
        for ( auto& q : qs )
        {
            if ( &q != this )
            {
                siblings.emplace_back( &q );
            }
        }
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

template < WorkDistStrategy workDistStrategy >
struct Pool
{
    std::vector< Queue< StealingStrategy::LookNext > > qs{};

    Pool() = default;

    explicit Pool( size_t size ) : qs( size )
    {
        for ( auto& q : qs )
        {
            q.connectWithSiblings( qs );
            q.initialize();
        }
    }

    [[nodiscard]] size_t chooseQueue()
    {
        if constexpr ( workDistStrategy == WorkDistStrategy::Random )
        {
            static std::mt19937 eng{ std::random_device()() };
            static std::uniform_int_distribution< size_t > dist;
            return dist( eng ) % qs.size();
        }
        else
        {
            static size_t idx{ 0 };
            auto ret = idx;
            idx = ( idx + 1 ) / qs.size();
            return ret;
        }
    }

    void post( const std::function< void() >& fun )
    {
        if ( !qs.empty() )
        {
            qs[ chooseQueue() ].enqueue( fun );
        }
    }

    void join()
    {
        for ( auto& q : qs )
        {
            q.done();
        }
    }

    ~Pool()
    {
        join();
    }
};

int fib( int n )
{
    return ( n > 2 ) ? fib( n - 1 ) + fib( n - 2 ) : 1;
}

void test_functionality()
{
    using Pool = Pool< WorkDistStrategy::Random >;
    // empty pool should not cause any issue for the implicit join()
    Pool emptyP;

    Pool p{ 12 };
    for ( auto i = 0; i < 80; ++i )
    {
        // work is distributed randomly (and may trigger randomized job-stealing)
        p.post( [ i ]() { fib( 25 + i % 18 ); } );
    }
    // explicit join
    p.join();
}

template < int numWorks, int baseWorkload, int maxWorkload >
void quick_bench()
{
    std::cout << "numWorks: " << numWorks << " base: " << baseWorkload << " max: " << maxWorkload
              << '\n';
    AutoTimer::Builder()
        .withLabel( "compare serial to pool" )
        .measure(
            //
            "serial",
            []() {
                for ( auto i = 0; i < numWorks; ++i )
                {
                    fib( baseWorkload + i % ( maxWorkload - baseWorkload ) );
                }
            } )
        .measure(
            //
            "pool 12",
            []() {
                Pool< WorkDistStrategy::Random > p( 12 );
                for ( auto i = 0; i < numWorks; ++i )
                {
                    p.post( [ i ]() { fib( baseWorkload + i % ( maxWorkload - baseWorkload ) ); } );
                }
                p.join();
            } );
}

int main()
{
    quick_bench< 240, 27, 38 >();
    quick_bench< 24, 38, 42 >();

    quick_bench< 233, 27, 38 >();
    quick_bench< 25, 38, 42 >();

    return 0;
}