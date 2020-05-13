//
// Created by weining on 13/5/20.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"

#include <thread>
#include <functional>

TEST_CASE ("pass arguments of prim types") {
    using namespace std;

    function<int(int, int)> f = [](int n, int m) { return n + m; };

    thread t(f, 1, 2);
    t.join();

}
