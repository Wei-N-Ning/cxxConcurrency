//
// Created by weining on 24/5/20.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "../stdthread/ThreadPool.hh"
#include <atomic>
#include <iostream>
#include <thread>

// in action 2nd P229
class Spinlock {
public:
    void lock() {
        while (flag.test_and_set(std::memory_order_acquire));
        std::printf("LO %llu\n", std::this_thread::get_id());
    }

    void unlock() {
        flag.clear(std::memory_order_release);
        std::printf("UN %llu\n", std::this_thread::get_id());
    }

private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
};

int fib(int n) {
    return (n == 1 or n == 0) ? 1 : fib(n - 1) + fib(n - 2);
}

// P/230
// This code doesn’t call any blocking functions; lock() keeps looping
// until the call to test_and_set() returns false.
// This is why it gets the name spin lock—the code “spins” around the loop.

//  Instead, you need to know which
//(if any) of the more specific terms defined here apply. These are
// Obstruction-Free
// If all other threads are paused, then any given thread will complete
// its operation in a bounded number of steps.
// Lock-Free
// If multiple threads are operating on a data structure, then after a
// bounded number of steps one of them will complete its operation.
// Wait-Free
// Every thread operating on a data structure will complete its operation
// in a bounded number of steps, even if other threads are also operating on
// the data structure
TEST_CASE ("") {
    Spinlock spl;
    ThreadPoolGithub::ThreadPool tp{std::thread::hardware_concurrency()};
    for (auto i = 0; i < 16; ++i) {
        tp.enqueue([&spl](auto work) {
            spl.lock();
            fib(work);
            spl.unlock();
        }, i);
    }
}