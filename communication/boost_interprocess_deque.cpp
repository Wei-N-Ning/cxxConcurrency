//
// Created by weining on 17/6/20.
//

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/deque.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <string>
#include <optional>
#include <iostream>

// c++ boost app dev P/308
// you must not use STL container with boost's shared memory segment

// see also:
// https://theboostcpplibraries.com/boost.interprocess-managed-shared-memory
// https://www.boost.org/doc/libs/master/doc/html/interprocess.html#interprocess.intro.introduction_building_interprocess

struct Task
{
    explicit Task( int x ) : value( x )
    {
    }

    int value{};
};

class Queue
{
public:
    using managed_shared_memory_t = boost::interprocess::managed_shared_memory;
    using allocator_t =
        boost::interprocess::allocator< Task, managed_shared_memory_t::segment_manager >;
    explicit Queue()
        : segment_( boost::interprocess::open_or_create, "work-queue", 1024 * 1024 * 64 )
        , allocator_( segment_.get_segment_manager() )
        , tasks_( *segment_.find_or_construct< deque_t >( "work-queue:deque" )( allocator_ ) )
        , mutex_( *segment_.find_or_construct< mutex_t >( "work-queue:mutex" )() )
        , cond_( *segment_.find_or_construct< condition_t >( "work-queue:condition" )() )
    {
    }

    std::optional< Task > try_pop_task()
    {
        std::optional< Task > ret;
        scoped_lock_t lock( mutex_ );
        if ( !tasks_.empty() )
        {
            ret = tasks_.front();
            tasks_.pop_front();
        }
        return ret;
    }

    void push_task(int x)
    {
        scoped_lock_t lock( mutex_ );
        tasks_.push_back( Task{x} );
    }

private:
    managed_shared_memory_t segment_;
    const allocator_t allocator_;

    using deque_t = boost::interprocess::deque< Task, allocator_t >;
    using mutex_t = boost::interprocess::interprocess_mutex;
    using condition_t = boost::interprocess::interprocess_condition;
    using scoped_lock_t = boost::interprocess::scoped_lock< mutex_t >;

    deque_t& tasks_;
    mutex_t& mutex_;
    boost::interprocess::interprocess_condition& cond_;
};

void server()
{
    Queue q;
    for (auto i = 0; i < 1999; ++i) {
        q.push_task(i);
        sleep(1);
    }
}

void client()
{
    Queue q;
    while (true) {
        sleep(1);
        if (auto t = q.try_pop_task(); t.has_value()) {
            std::cout << "task: " << t->value << '\n';
        } else {
            std::cout << "idle...\n";
        }
    }
}

int main( int argc, char* argv[] )
{
    if ( argc == 2 and std::string( argv[ 1 ] ) == "server" )
    {
        server();
    }
    else if ( argc == 2 and std::string( argv[ 1 ] ) == "client" )
    {
        client();
    }
    return 0;
}
