//
// Created by weining on 14/5/20.
//

#ifndef C_CONCURRENCYEXAMPLES_THREAD_POOL_H
#define C_CONCURRENCYEXAMPLES_THREAD_POOL_H

#include <thread>
#include <deque>
#include <algorithm>

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

    void join() {
        while (!pool.empty()) {
            if (pool.front().joinable()) {
                pool.front().join();
            }
            pool.pop_front();
        }
    }

    ~ThreadPool() {
        join();
    }
};

#endif //C_CONCURRENCYEXAMPLES_THREAD_POOL_H