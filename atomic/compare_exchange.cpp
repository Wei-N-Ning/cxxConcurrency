//
// Created by weining on 24/5/20.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include <atomic>

// in action 2nd P/158
//compare-exchange operation is the cornerstone of programming with atomic types;
//it compares the value of the atomic variable with a supplied expected value and
//stores the supplied desired value if they’re equal.
// if the values aren't equal, the expected value is updated with the value of
// the atomic variable

// On the other hand, compare_exchange_strong() is guaranteed to return false
//only if the value wasn't equal to the expected value.
TEST_CASE ("") {
    ;
}