#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

// the current status of parallel execution policy in C++17 is a MESS
// only intel's parallel stl and MSVC has full support
// I don't want to go into vendor lock.

// neither clang nor gcc supports as of version clang-9 and gcc 7.5

//#include <execution>
#include <vector>

TEST_CASE("par: all_of") {
    auto elems = new std::vector<int>(10000000, 1);
    elems->back() = 0;
    std::all_of(elems->cbegin(), elems->cend(), [](const auto &elem) { return elem == 1; });
    delete elems;
}
