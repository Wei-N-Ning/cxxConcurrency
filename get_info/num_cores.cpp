//
// Created by weining on 9/5/20.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <iostream>
#include <thread>

// c++ stl cookbook P/414

TEST_CASE ("") {
    using namespace std;
    // on gunship (4 cores x 2T/c), this shows 8 cores
    // naturally this is the same result as in Python, using multiprocessing.cpu_count()
    auto ncores{thread::hardware_concurrency()};
    cout << "num cores: " << ncores << endl;
}
