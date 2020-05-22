//
// Created by weining on 13/5/20.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"

#include <atomic>
#include <vector>
#include <memory>
#include <functional>

#include "../stdthread/thread_pool.hh"

// CAS:
// https://en.cppreference.com/w/cpp/atomic/atomic/compare_exchange
// also: https://en.wikipedia.org/wiki/Compare-and-swap
// function cas(p : pointer to int, old : int, new : int)
// the value to be pushed to the shared resource should have no dependency to
// some previous state
// in the case of the counter it DOES require the lineage therefore CAS doesn't
// really work here

class Counter {
public:
    void inc() {
        v++;
    }

    [[nodiscard]] int value() const {
        return v;
    }

private:
    std::atomic<int> v{0};
};

int fib(int n) {
    return (n == 1 or n == 0) ? 1 : fib(n - 1) + fib(n - 2);
}

TEST_CASE ("") {
    using namespace std;

    Counter c;
    constexpr int work_sz = 512;
    constexpr int workload = 34;
    vector<int> works(work_sz, workload);

    {
        ThreadPool tp;
        for_each(cbegin(works), cend(works), [&c, &tp](int n) {
            tp.do_work([&c](int n) {
                fib(n);
                c.inc();
            }, n);
        });
    }
    CHECK_EQ(work_sz, c.value());
}
