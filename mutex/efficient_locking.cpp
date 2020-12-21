//
// Created by weining on 22/12/20.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include <mutex>
#include <vector>

TEST_CASE( "use if-initialization to lock a mutex inside an if scope" )
{
    // c++ 17 the complete guide P/20
    // note:
    // - the lock must have a name (otherwise it will be immediately deleted)
    // - take advantage of c++ 17 template deduction to simplify the writing
    std::mutex m;
    std::vector xs( 10, 0 );

    {
        // some thread scope
        if ( std::unique_lock l{ m }; !xs.empty() )
        {
            xs.emplace_back( 1 );
        }
    }
}