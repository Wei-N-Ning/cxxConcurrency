//
// Created by weining on 24/5/20.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include <memory>
#include <atomic>
#include <vector>

// in action 2nd P/164
// use atomic free functions
TEST_CASE ("") {
    std::vector<int> v3{1, 2, 3};
    auto p = std::make_shared<std::vector<int>>(10, 2);
    {
        auto p1 = std::atomic_load(&p);
        CHECK_EQ(p1.use_count(), 2);
    }
    CHECK_EQ(p.use_count(), 1);
}

