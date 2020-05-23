//
// Created by weining on 23/5/20.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"

#include <algorithm>
#include <vector>
#include <thread>
#include <boost/asio.hpp>

// inspired by in action 2nd P/73
// thread-safe stack
// since I have worked on a lockfree stack, which uses the CAS idiom,
// I wonder what other techniques I can use to improve thread safety,
// and here is an experiment: to keep each "cell" in parallel so that
// each computation that runs on an arbitrary thread has one and only
// one piece of mutable data to interface
//
// data parallelism it is

constexpr int fib(int n) {
    return (n == 1 or n == 0) ? 1 : fib(n - 1) + fib(n - 2);
}

TEST_CASE ("") {
    std::size_t works = 128;
    std::vector<int> result(works);
    {
        boost::asio::thread_pool tp(std::thread::hardware_concurrency());
        std::for_each(
            std::begin(result), std::end(result),
            [&tp](auto &cell) {
                boost::asio::post(tp, [&cell]() { cell = fib(30); });
            }
        );
        tp.join();
    }
    auto total_computed = std::count_if(
        std::begin(result), std::end(result),
        [expected = fib(30)](auto cell) { return cell == expected; }
    );
    CHECK_EQ(total_computed, result.size());
}
