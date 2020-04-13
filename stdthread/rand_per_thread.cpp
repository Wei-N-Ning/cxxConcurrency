//
// Created by wein on 25/10/18.
//
// source:
// master cxx mt L387

// mt19937:
// http://www.cplusplus.com/reference/random/mt19937/
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <iostream>
#include <random>
#include <thread>

int randGen(int min_, int max_) {
    using namespace std;

    // every thread will create its own generator
    // a hash of the internal thread identifier is used as a seed for
    // the generator
    static thread_local mt19937 generator(
        hash<thread::id>()(
            this_thread::get_id()
        )
    );
    uniform_int_distribution<int> distribution(min_, max_);
    return distribution(generator);
}

TEST_CASE("thread has its own generator") {
    // note how the number remains the same between executions
    // (main thread only)
    std::cout
    << randGen(1, 0x1111111)
    << randGen(1, 0x1111111)
    << randGen(1, 0x1111111)
    << std::endl;
}