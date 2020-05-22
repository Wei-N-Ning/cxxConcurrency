//
// Created by weining on 22/5/20.
//

// to test the assumption that
// std::async is fast(er)
// weining@weining-ubuntu:build$ time ./async/async_future_perf seq
//
//real	0m23.117s <----------
//user	0m23.113s
//sys	0m0.004s
//weining@weining-ubuntu:build$ time ./async/async_future_perf thread-pool
//
//real	0m4.399s  <----------
//user	0m50.413s
//sys	0m0.004s
//weining@weining-ubuntu:build$ time ./async/async_future_perf future-pool
//
//real	0m4.521s  <----------
//user	0m51.918s
//sys	0m0.008s
//weining@weining-ubuntu:build$ time ./async/async_future_perf future
//
//real	0m4.502s  <----------
//user	0m53.434s
//sys	0m0.021s

// std::async may use a thread pool
// gcc and clang DOES NOT use thread pool,
// see: https://stackoverflow.com/questions/55873027/c-which-thread-pool-is-cppreference-com-talking-about
// can also confirm this by looking into the gcc header source code

// the verdict:
// boost::asio::thread-pool uses only 12 threads
// this one (https://github.com/progschj/ThreadPool) also does the same (see his blog)
// the incorrect thread-pool and future-pool I wrote "happens" to also use 12 threads but it still calls N times
// the thread constructor
// lastly, the "future" scenario creates 128 threads (128 is the number of works)

#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <iostream>
#include <thread>

#include "../stdthread/thread_pool.hh"
#include "future_pool.hh"
#include "../stdthread/ThreadPool.hh"

#include <boost/asio.hpp>

int fib_(int n) {
    return (n == 1 or n == 0) ? 1 : fib_(n - 1) + fib_(n - 2);
}

int fib(int n) { return fib_(n); }

int fib_fx(int n) {
    // fib now has side effects lol
    auto o = fib_(n);
    using namespace std;
    cout << "pthread id: " << pthread_self() << endl;
    return o;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        return 1;
    }
    int works[128] = {};
    std::fill(std::begin(works), std::end(works), 40);

    if (std::strcmp("thread-pool", argv[1]) == 0) {
        ThreadPool tp;
        std::for_each(
            std::begin(works), std::end(works),
            [&tp](auto work) {
                tp.do_work(fib, work);
            });
    } else if (std::strcmp("future-pool", argv[1]) == 0) {
        FuturePool fp;
        std::for_each(
            std::begin(works), std::end(works),
            [&fp](auto work) {
                fp.do_work([work]() { fib(work); });
            });
    } else if (std::strcmp("future", argv[1]) == 0) {
        std::vector<std::future<int>> workers{};
        workers.reserve(std::size(works));
        std::transform(
            std::begin(works), std::end(works), std::back_inserter(workers),
            [](auto work) {
                return std::async(std::launch::async, fib, work);
            });
        for (auto &worker : workers) {
            worker.get();
        }
    } else if (std::strcmp("seq", argv[1]) == 0) {
        for (auto work : works) {
            fib(work);
        }
    } else if (std::strcmp("boost-thread-pool", argv[1]) == 0) {
        boost::asio::thread_pool tp(std::thread::hardware_concurrency());
        for (auto work : works) {
            boost::asio::post(tp, [work]() { fib(work); });
        }
        tp.join();
    } else if (std::strcmp("ThreadPool", argv[1]) == 0) {
        ThreadPoolGithub::ThreadPool tp(std::thread::hardware_concurrency());
        std::vector<std::future<int>> futs{};
        futs.reserve(std::size(works));
        for (auto work : works) {
            futs.push_back(tp.enqueue(fib, work));
        }
        for (auto &fut : futs) {
            fut.get();
        }
    }
    else {
        throw std::runtime_error("accepted keywords are: \n"
                                 "thread-pool\n"
                                 "future-pool\n"
                                 "future\n"
                                 "seq\n"
                                 "boost-thread-pool\n"
                                 "ThreadPool\n");
    }
    return 0;
}
