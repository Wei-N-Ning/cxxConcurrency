//
// Created by wein on 26/10/18.
//

// source
// master cxx mt L952 (synchronization)

// observe the runtime syscalls:
// mprotect(0x7fa739960000, 2093056, PROT_NONE) = 0
//mprotect(0x7fa73973c000, 2097152, PROT_NONE) = 0
//mprotect(0x7fa73993c000, 16384, PROT_READ) = 0
//mprotect(0x7fa739b5f000, 4096, PROT_READ) = 0
//mprotect(0x562c0a583000, 4096, PROT_READ) = 0
//mprotect(0x7fa739d8c000, 4096, PROT_READ) = 0
//mprotect(0x7fa738d55000, 8388608, PROT_READ|PROT_WRITE) = 0
//mprotect(0x7fa738554000, 8388608, PROT_READ|PROT_WRITE) = 0
//mprotect(0x7fa737d53000, 8388608, PROT_READ|PROT_WRITE) = 0
//mprotect(0x7fa737552000, 8388608, PROT_READ|PROT_WRITE) = 0
//mprotect(0x7fa736d51000, 8388608, PROT_READ|PROT_WRITE) = 0
//mprotect(0x7fa736550000, 8388608, PROT_READ|PROT_WRITE) = 0
//mprotect(0x7fa735d4f000, 8388608, PROT_READ|PROT_WRITE) = 0
//mprotect(0x7fa73554e000, 8388608, PROT_READ|PROT_WRITE) = 0

#include <cassert>

#include <pthread.h>

namespace {

constexpr int NUM_THREADS = 8;
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

void* worker(void* arg) {
    assert(0 == pthread_rwlock_rdlock(&rwlock));
    // missing unlock will cause the wrlock() to block forever
    assert(0 == pthread_rwlock_unlock(&rwlock));

    assert(0 == pthread_rwlock_wrlock(&rwlock));
    assert(0 == pthread_rwlock_unlock(&rwlock));
    return nullptr;
}

void execute() {
    ::pthread_t threads[NUM_THREADS];
    int threadArgs[NUM_THREADS];
    int retcode;

    size_t idx = 0;
    for (const auto& arg : threadArgs) {
        retcode = pthread_create(
            &threads[idx], nullptr, worker, (void *) (&arg)
        );
        assert(0 == retcode);
        ++idx;
    }

    for (const auto& th : threads) {
        pthread_join(th, nullptr);
    }
}

}

void RunTinyTests();

void test_execute() {
    execute();
}

int main(int argc, char **argv) {
    RunTinyTests();
    return 0;
}
