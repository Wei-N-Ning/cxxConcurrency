//
// Created by weining on 13/5/20.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"

#include <thread>
#include <functional>
#include <vector>

// mastering c++ programming: L5658

TEST_CASE ("pass arguments of prim types") {
    using namespace std;

    function<int(int, int)> f = [](int n, int m) { return n + m; };

    thread t(f, 1, 2);
    t.join();

}

TEST_CASE ("pass-by-reference") {
    using namespace std;

    vector<int> v{1};

    function<int(vector<int> &)> f = [](vector<int> &v) {
        if (v.size() == 1) {
            v[0] = 10;
            return 1;
        }
        return 0;
    };

    // L5658
    // thread ctor copies the values passed to it and then passes them to the
    // thread function with rvalue reference.
    // this is done to work with move-only types
    // (can 't pass rvalue to a function that expects a non-constant ref)
    // use std::ref
    thread t(f, ref(v));
    t.join();

    CHECK_EQ(10, v[0]);
}