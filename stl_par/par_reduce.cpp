#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <execution>
#include <vector>

TEST_CASE("par: all_of") {
    auto elems = new std::vector<int>(10000000, 1);
    elems->back() = 0;
    std::all_of(elems->cbegin(), elems->cend(), [](const auto &elem) { return elem == 1; });
    delete elems;
}
