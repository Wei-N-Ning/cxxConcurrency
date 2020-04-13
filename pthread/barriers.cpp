//
// Created by wein on 26/10/18.
//

// source
// master cxx mt L962
// all of these (threads) have to reach the barrier before
// any of them can proceed past it

// ----> |
// ----> |
// ----> |
//         -->
//         ->
//         ----->

// in the barrier initialization function, the thread count
// is specified. Only once all of these threads have called
// the barrier object, using the pthread_barrier_wait()
// function will they continue executing

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <cassert>

#include <pthread.h>

namespace {

pthread_barrier_t barrier;
constexpr int NUM_THREADS = 4;

void* worker(void* arg) {
    pthread_barrier_wait(&barrier);
    return nullptr;
}

void execute() {
    // if the size passed to the initializer does not match
    // the total number of threads, all workers can be blocked
    // forever
    pthread_barrier_init(&barrier, nullptr, NUM_THREADS);

    ::pthread_t threads[NUM_THREADS];
    int retcode;

    for (auto& th : threads) {
        retcode = pthread_create(
            &th, nullptr, worker, nullptr
        );
        assert(0 == retcode);
    }

    for (auto& th : threads) {
        pthread_join(th, nullptr);
    }
}

}


TEST_CASE("test barrier") {
    execute();
}

