//
// Created by weining on 13/5/20.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"

#include <thread>
#include <functional>
#include <iostream>
#include <map>
#include <vector>

// mastering c++ programming L5624
// also explained in effective modern c++: make sure a thread is joined in all paths

struct Joinable {
    std::thread t{};

    explicit Joinable(std::thread thread) : t{std::move(thread)} {}

    ~Joinable() { if (t.joinable()) t.join(); }
};

TEST_CASE ("RAII joinable thread") {
    using namespace std;

    function<int(int)> fib = [&fib](int n) {
        if (n == 1 or n == 2) return 1;
        return fib(n - 1) + fib(n - 2);
    };

    int out{0};
    {
        Joinable jt(
            thread([&fib, &out, sz = 42] { out = fib(sz); })
        );
    }
    cout << out << endl;
}
