//
// Created by weining on 22/5/20.
//

#ifndef C_CONCURRENCYEXAMPLES_FUTURE_POOL_HH
#define C_CONCURRENCYEXAMPLES_FUTURE_POOL_HH

#include <future>
#include <deque>

struct FuturePool {
    std::deque <std::future<void>> pool{};
    std::size_t max_size{std::thread::hardware_concurrency()};

    FuturePool() = default;

    explicit FuturePool(std::size_t sz) : max_size(sz) {}

    template<typename F>
    void do_work(F f) {
        if (pool.size() >= max_size) {
            pool.front().get();
            pool.pop_front();
        }
        auto fut = std::async(std::launch::async, f);
        pool.push_back(std::move(fut));
    }

    ~FuturePool() {
        while (!pool.empty()) {
            pool.front().get();
            pool.pop_front();
        }
    }
};

#endif //C_CONCURRENCYEXAMPLES_FUTURE_POOL_HH
