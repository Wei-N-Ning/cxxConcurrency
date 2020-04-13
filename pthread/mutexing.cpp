//
// Created by wein on 26/10/18.
//

// source
// master cxx mt L893 (pthread's mutex)

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <iostream>
#include <iterator>
#include <vector>
#include <cassert>

#include <pthread.h>

namespace {

constexpr int NUM_THREADS = 8;
pthread_mutex_t func_mutex = PTHREAD_MUTEX_INITIALIZER;
using store_t = std::vector<int>;

void* compute(void *arg) {
    using namespace std;
    auto store = static_cast<store_t *>(arg);

    static int num = 1;

    pthread_mutex_lock(&func_mutex);
    //
    store->push_back(num);
    num += 1;
    //
    pthread_mutex_unlock(&func_mutex);

    // move num += 1 out of the critical section and...
    // ticc test mutexing.cpp  -- -lpthread -fsanitize=thread
    // thread sanitizer will catch the data race!

    return nullptr;
}

int execute() {
    ::pthread_t threads[NUM_THREADS];
    int retcode;
    store_t store;
    assert(store.empty());

    for (auto& th : threads) {
        retcode = pthread_create(
            &th, nullptr, compute, (void *) (&store)
        );
        assert(0 == retcode);
    }

    // run this with strace...
    // the number of calls to futex is expected to be arbitrary:
    // futex(0x7f81959ac07c, FUTEX_WAKE_PRIVATE, 2147483647) = 0
    // futex(0x7f81959ac088, FUTEX_WAKE_PRIVATE, 2147483647) = 0
    // futex(0x7f818dffb9d0, FUTEX_WAIT, 11385, NULL) = 0
    for (const auto& th : threads) {
        pthread_join(th, nullptr);
    }

    assert(NUM_THREADS == store.size());
    std::copy(store.cbegin(), store.cend(),
        std::ostream_iterator<int>(std::cout, ",  "));
    return 0;
}

}

TEST_CASE("mutex") {
    execute();
}

