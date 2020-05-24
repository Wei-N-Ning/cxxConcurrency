//
// Created by weining on 23/5/20.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include <shared_mutex>
#include <thread>
#include <map>
#include <boost/asio.hpp>

// in action 2nd P/91
// This new kind of mutex is typically called a reader-writer mutex, because
// it allows for two different kinds of usage:

// exclusive access by a single “writer” thread or shared,
// and concurrent access by multiple “reader” threads.

// The C++17 Standard Library provides two such mutexes out of the box,
// std::shared_mutex (no additional operations, faster), c++17
// and std::shared_timed_mutex (with additional operations), c++14
// boost provides both

// see also:
// https://en.cppreference.com/w/cpp/thread/shared_mutex

struct Resource {
    std::shared_mutex shm;
    std::map<int, int> m{};

    int read(int k) {
        // uses std::shared_lock<> to protect it for shared, read-only access
        // multiple threads can therefore call find() simultaneously
        std::shared_lock<std::shared_mutex> l(shm);
        if (auto it = m.find(k); it != m.end()) {
            return it->second;
        }
        return -1;
    }

    void write(int k, int v) {
        // uses lock_guard/unique_lock to provide exclusive access while the
        // map is update; not only are other threads prevented from doing
        // updates in a call to write(), but threads that call read() are
        // blocked too
        std::unique_lock<std::shared_mutex> l(shm);
        m[k] = v;
    }
};

constexpr int fib(int n) {
    return (n == 1 or n == 0) ? 1 : fib(n - 1) + fib(n - 2);
}

TEST_CASE ("") {
    Resource res;
    CHECK_EQ(res.m.size(), 0);
    {
        boost::asio::thread_pool tp(std::thread::hardware_concurrency());
        for (auto i = 0; i < 34; ++i) {
            boost::asio::post(
                tp,
                [&res, i]() {
                    res.read(i);
                    auto o = fib(i);
                    res.write(i, o);
                }
            );
        }
        tp.join();
    }
    CHECK_EQ(res.m.size(), 34);
    constexpr int expected = fib(21);
    CHECK_EQ(res.read(21), expected);
}
