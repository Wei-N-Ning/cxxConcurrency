//
// Created by weining on 31/12/20.
//

#include <unordered_map>
#include <mutex>
#include <optional>
#include <random>
#include <deque>
#include <functional>

#include <autotimer.hh>

// inspired by
// https://www.youtube.com/watch?v=zULU6Hhp42w&feature=emb_title
// 25:00
// any mutex can be transformed to a serial queue
// set() -> void
//      async(do_set_operation)
// set() becomes non-blocking no matter how large the size of the data is

// get() -> future
//      async(do_get_operation*)
// get() will block

// the serial queue must guarantee the order of these set/get operations

struct StoreWithMutex
{
    std::mutex mu;
    std::unordered_map< std::string, std::string > tb{};

    void clear()
    {
        tb.clear();
    }

    void set( std::string key, std::string value )
    {
        std::unique_lock l{ mu };
        tb.emplace( std::move( key ), std::move( value ) );
    }

    auto get( const std::string& key ) -> std::optional< std::string >
    {
        std::unique_lock l{ mu };
        if ( auto it = tb.find( key ); it != tb.end() )
        {
            return it->second;
        }
        return std::nullopt;
    }
};

using Task = std::function< void() >;

struct Queue
{
    std::deque< Task > tasks{};
    std::mutex mu;

    void clear()
    {
        tasks.clear();
    }

    void enqueue( const Task& task )
    {
        std::lock_guard l{ mu };
        tasks.emplace_back( task );
    }

    auto dequeue() -> std::optional< Task >
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
};

template < typename >
struct TypeTester;

struct StoreWithQueue
{
    std::unordered_map< std::string, std::string > tb{};
    Queue q{};

    void clear()
    {
        tb.clear();
        q.clear();
    }

    void set( std::string key, std::string value )
    {
        q.enqueue( [ &key, &value, tb = &tb ]() {
            //
            tb->emplace( std::move( key ), std::move( value ) );
        } );
    }

    auto get( const std::string& key ) -> std::optional< std::string >
    {
        return std::nullopt;
    }
};

std::string randomStr( int length )
{
    static std::mt19937 eng{ std::random_device()() };
    static std::uniform_int_distribution< char > dist{ 'A', 'z' };
    std::string o( length, '\0' );
    std::generate( o.begin(), o.end(), [ & ]() { return dist( eng ); } );
    return o;
}

int main()
{
    StoreWithMutex storeWithMutex{};
    StoreWithQueue storeWithQueue{};
    std::string text{ "there is a cow there is a cow there is a cow there is a cow" };
    AutoTimer::Builder()
        .withLabel( "compare mutex to serial operation queue" )
        .withMultiplier( 10 )
        .withInit( [ & ]() {
            storeWithMutex.clear();
            storeWithQueue.clear();
        } )
        .measure(
            //
            "mutex ht",
            [ & ]() {
                for ( auto i = 0; i < 1000; ++i )
                {
                    storeWithMutex.set( randomStr( 8 ), text );
                    storeWithMutex.get( randomStr( 8 ) );
                }
            } )
        .measure(
            //
            "queue ht",
            [ & ]() {
                for ( auto i = 0; i < 1000; ++i )
                {
                    storeWithQueue.set( randomStr( 8 ), text );
                    storeWithQueue.get( randomStr( 8 ) );
                }
            } );
    return 0;
}
