//
// Created by weining on 21/5/20.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include <thread>

// in action 2nd P/52
// transfer thread ownership using move semantic
// Putting std::thread objects in a std::vector is a step toward automating
// the management of those threads: rather than creating separate variables
// for those threads and joining with them directly, they can be treated as
// a group.

struct ScopedThread {
    std::thread t{};

    explicit ScopedThread(std::thread t_) : t{std::move(t_)} {}

    ~ScopedThread() {
        if (t.joinable()) t.join();
    }

    ScopedThread(const ScopedThread &) = delete;

    ScopedThread &operator=(const ScopedThread &) = delete;
};

TEST_CASE ("") {
    ;
}