//
// Created by weining on 13/5/20.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"

#include <algorithm>
#include <deque>
#include <vector>
#include <thread>
#include <map>

// mastering c++ programming L5676

int fib(int n) {
    return (n == 0 or n == 1) ? 1 : fib(n - 1) + fib(n - 2);
}

struct ThreadPool {
    std::deque<std::thread> pool{};
    std::size_t max_size{std::thread::hardware_concurrency()};

    ThreadPool() = default;
    explicit ThreadPool(std::size_t sz): max_size(sz) {}

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

// --- proof of concept ---

//TEST_CASE ("") {
//    using namespace std;
//
//    deque<thread> pool;
//    auto max_size{thread::hardware_concurrency()};
//
//    vector<int> workload(40, 30);
//
//    for_each(
//        cbegin(workload), cend(workload),
//        [&pool, max_size](int work) {
//            if (pool.size() < max_size) {
//                pool.emplace_back(fib, work);
//            } else {
//                pool.front().join();
//                pool.pop_front();
//                pool.emplace_back(fib, work);
//            }
//        }
//    );
//    while (!pool.empty()) {
//        pool.front().join();
//        pool.pop_front();
//    }
//}

// one core to all cores speed up: 4.771
TEST_CASE ("deque-based thread pool") {
    using namespace std;
    ThreadPool tp;

    vector<int> workload(40, 40);
    for_each(begin(workload), end(workload),
             [&tp](int work) { tp.do_work(fib, work); });
}
