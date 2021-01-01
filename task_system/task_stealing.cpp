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

#include <autotimer.hh>

// this example is based on local_task_queue.cpp and adds task-stealing capability
//

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
    std::vector< Queue* > siblings{};

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

struct Pool
{
    std::vector< Queue > qs{};
    std::mt19937 eng{ std::random_device()() };
    std::uniform_int_distribution< size_t > dist;

    Pool() = default;

    explicit Pool( size_t size ) : qs( size )
    {
        dist.param( decltype( dist )::param_type( 0, qs.size() - 1 ) );
    }

    [[nodiscard]] size_t randomIdx()
    {
        return dist( eng );
    }

    void post( const std::function< void() >& fun )
    {
        if ( !qs.empty() )
        {
            qs[ randomIdx() ].enqueue( fun );
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

int main()
{
    Pool emptyP;

    Pool p{ 4 };
    for ( auto i = 0; i < 40; ++i )
    {
        p.post( [ i ]() { fib( 30 + i % 10 ); } );
    }
    p.join();
    return 0;
}