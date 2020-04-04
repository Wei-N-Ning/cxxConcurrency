#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

// NOTE: 
// this example was inspired by FP with C++ book, where it showed off
// the parallel execution policy, part of C++17, but only implemented 
// and supported in MSVC
// I was very excited to see the working code but quickly realized that
// neither GCC nor CLANG had kept theirs up to date
// Intel, on the other hand, released their parallel_stl library that 
// implemented the parallel version of stl higher-order functions, but
// I was not yet able to build and install it on Ubuntu 18 (gunship)

// the current status of parallel execution policy in C++17 is a MESS
// only intel's parallel stl and MSVC has full support
// I don't want to go into vendor lock.

// neither clang nor gcc supports as of version clang-9 and gcc 7.5

#include <vector>

TEST_CASE("par: all_of") {
    auto elems = new std::vector<int>(10000000, 1);
    elems->back() = 0;
    std::all_of(elems->cbegin(), elems->cend(), [](const auto &elem) { return elem == 1; });
    delete elems;
}
