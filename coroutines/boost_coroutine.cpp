//
// Created by weining on 18/6/20.
//

#include <boost/coroutine2/all.hpp>
#include <iostream>

// mentioned in c++ boost app dev P/314
// however coroutines is deprecated and superseded by coroutines2
// see: https://theboostcpplibraries.com/boost.coroutine
// good read:
// the motivation behind boost::coroutine
// https://www.boost.org/doc/libs/1_73_0/libs/coroutine2/doc/html/coroutine2/motivation.html
using namespace boost::coroutines2;

void cooperative( coroutine< void >::push_type& sink )
{
    std::cout << "Hello";
    sink();
    std::cout << "world";
}

void demo_cooperative()
{
    coroutine< void >::pull_type source{ cooperative };
    std::cout << ", ";
    source();
    std::cout << "!\n";
}

int main( int argc, char* argv[] )
{
    demo_cooperative();
    return 0;
}