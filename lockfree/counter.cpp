//
// Created by weining on 13/5/20.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"

#include <thread>
#include <atomic>
#include <vector>

int fib(int n) {
    return (n == 1 or n == 0) ? 1 : fib(n - 1) + fib(n - 2);
}

struct Counter {

};

TEST_CASE ("") {
    ;
}
