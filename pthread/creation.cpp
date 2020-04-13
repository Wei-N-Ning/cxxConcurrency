//
// Created by wein on 26/10/18.
//

// source:
// master cxx mt L800 (pthread interface)
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <cassert>

#include <pthread.h>

namespace {

constexpr int NUM_THREADS = 8;

void* compute(void *arg) {
    //
    return nullptr;
}

int execute() {
    ::pthread_t threads[NUM_THREADS];
    int threadArgs[NUM_THREADS];
    int retcode;

    size_t idx = 0;
    for (const auto& arg : threadArgs) {
        retcode = pthread_create(
            &threads[idx], nullptr, compute, (void *) (&arg)
        );
        assert(0 == retcode);
        ++idx;
    }

    for (const auto& th : threads) {
        pthread_join(th, nullptr);
    }

    return 0;
}

}

TEST_CASE("create pthread") {
    execute();
}

