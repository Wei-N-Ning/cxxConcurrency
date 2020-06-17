//
// Created by weining on 17/6/20.
//

#include <boost/interprocess/managed_shared_memory.hpp>
#include <atomic>
#include <string>
#include <cassert>
#include <iostream>

// c++ boost app dev P/302
// P/304
// mutex does not work in shared memory (hence the assert() in main())
// if the shared variable (the atomic int) is not lockfree, I get an undefined behavior
using AtomicT = std::atomic< int >;

struct Segment
{
    boost::interprocess::managed_shared_memory seg;

    Segment()
    {
        seg = boost::interprocess::managed_shared_memory(
            boost::interprocess::open_or_create, "shm-cache", 1024 );
    }

    ~Segment()
    {
        seg.destroy< AtomicT >( "shm-counter" );
        boost::interprocess::shared_memory_object::remove( "shm-cache" );
    }
};

void server()
{
    // this creates a (binary) file at:
    // /dev/shm/shm-cache
    // I can even probe the content sof this file, xxd /dev/shm/shm-cache
    // the size of the segment must be big enough to fit the boost::interprocess specific data
    // boost rounds it to the nearest larger value
    Segment segment;
    AtomicT& a = *segment.seg.find_or_construct< AtomicT >( "shm-counter" )( 0 );
    assert( std::atomic_is_lock_free( &a ) );

    for ( char ch; std::cin >> ch; )
    {
        a++;
        sleep( 1 );
        std::cout << "press ctrl + d to stop: " << a << '\n';
    }
}

void client()
{
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, "shm-cache", 1024 );
    AtomicT& a = *segment.find_or_construct< AtomicT >( "shm-counter" )( 0 );
    while ( true )
    {
        a++;
        sleep( 1 );
        std::cout << a << '\n';
    }
}

int main( int argc, char* argv[] )
{
    if ( argc == 2 and std::string( argv[ 1 ] ) == "client" )
    {
        client();
    }
    else if ( argc == 2 and std::string( argv[ 1 ] ) == "server" )
    {
        server();
    }
    return 0;
}
