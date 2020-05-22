//
// Created by weining on 22/5/20.
//

#ifndef C_CONCURRENCYEXAMPLES_THREAD_POOL_HH
#define C_CONCURRENCYEXAMPLES_THREAD_POOL_HH

#include <thread>
#include <deque>

struct ThreadPool {
    std::deque<std::thread> pool{};
    std::size_t max_size{std::thread::hardware_concurrency()};

    ThreadPool() = default;
    explicit ThreadPool(std::size_t sz): max_size(sz) {}

    template<typename... Ts>
    void do_work(Ts &&... args) {
        if (pool.size() >= max_size) {
            pool.front().join();
            pool.pop_front();
        }
        pool.emplace_back(std::forward<Ts>(args)...);
    }

    void join() {
        while (!pool.empty()) {
            pool.front().join();
            pool.pop_front();
        }
    }

    ~ThreadPool() {
        join();
    }
};

#endif //C_CONCURRENCYEXAMPLES_THREAD_POOL_HH
