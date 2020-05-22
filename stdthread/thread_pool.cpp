//
// Created by weining on 13/5/20.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"

#include "thread_pool.hh"

// mastering c++ programming L5676

int fib(int n) {
    return (n == 0 or n == 1) ? 1 : fib(n - 1) + fib(n - 2);
}

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
    vector<int> workload(40, 40);
    ThreadPool tp;
    for_each(begin(workload), end(workload),
             [&tp](int work) { tp.do_work(fib, work); });
}
