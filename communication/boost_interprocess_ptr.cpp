//
// Created by weining on 18/6/20.
//

#include <boost/interprocess/offset_ptr.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <string>
#include <iostream>
#include <cassert>

struct Node
{
    boost::interprocess::offset_ptr< int > pointer_{ nullptr };
    int value{};
};

void server()
{
    auto seg = boost::interprocess::managed_shared_memory(
        boost::interprocess::open_or_create, "shm-cache", 1024 );
    Node &ref = *seg.find_or_construct<Node>("Node")();
    ref.value = 999;
    ref.pointer_ = &ref.value;

    assert(ref.pointer == &ref.value);
}

int main( int argc, char* argv[] )
{
    server();
    return 0;
}