//
// Created by weining on 13/5/20.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"

#include <thread>
#include <atomic>
#include <vector>
#include <memory>
#include <functional>
#include <deque>

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

struct ThreadPool {
    std::deque<std::thread> pool{};
    std::size_t max_size{std::thread::hardware_concurrency()};

    ThreadPool() = default;

    explicit ThreadPool(std::size_t sz) : max_size(sz) {}

    template<typename... Ts>
    void do_work(Ts &&... args) {
        if (pool.size() < max_size) {
            pool.emplace_back(std::forward<Ts>(args)...);
        } else {
            pool.front().join();
            pool.pop_front();
            pool.emplace_back(std::forward<Ts>(args)...);
        }
    }

    ~ThreadPool() {
        while (!pool.empty()) {
            pool.front().join();
            pool.pop_front();
        }
    }
};

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
