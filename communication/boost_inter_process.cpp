//
// Created by weining on 17/6/20.
//

#include <boost/interprocess/managed_shared_memory.hpp>
#include <atomic>
#include <string>
#include <cassert>
#include <ctime>
#include <iostream>

// c++ boost app dev P/302

using AtomicT = std::atomic< int >;

void server()
{
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, "shm-cache", 1024 );
    AtomicT& a = *segment.find_or_construct< AtomicT >( "shm-counter" )( 0 );
    assert( std::atomic_is_lock_free( &a ) );

    while (true) {
        a++;
        sleep(1);
        std::cout << a << '\n';
    }
}

void client()
{
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, "shm-cache", 1024 );
    AtomicT& a = *segment.find_or_construct< AtomicT >( "shm-counter" )( 0 );
    while (true) {
        a++;
        sleep(1);
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
