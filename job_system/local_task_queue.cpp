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

int main()
{
    std::vector< Queue > children( 8 );

    for ( int i = 0; i < 30; ++i )
    {
        for ( auto& child : children )
        {
            child.enqueue( [ idx = i ]() {
                //
                fib( 20 + idx % 10 );
            } );
        }
    }
    return 0;
}
