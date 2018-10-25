#!/usr/bin/env bash

# inspired by master cxx mt L678
# what happens from hardware and OS' perspective,
# when an std::thread is created (executed) and joined?

# the result can look like:
# clone(child_stack=0x7f631f9b7fb0, flags=CLONE_VM|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_THREAD|CLONE_SYSVSEM|CLONE_SETTLS|CLONE_PARENT_SETTID|CLONE_CHILD_CLEARTID, parent_tidptr=0x7f631f9b89d0, tls=0x7f631f9b8700, child_tidptr=0x7f631f9b89d0) = 12419
# clone(child_stack=0x7f631f1b6fb0, flags=CLONE_VM|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_THREAD|CLONE_SYSVSEM|CLONE_SETTLS|CLONE_PARENT_SETTID|CLONE_CHILD_CLEARTID, parent_tidptr=0x7f631f1b79d0, tls=0x7f631f1b7700, child_tidptr=0x7f631f1b79d0) = 12420
# futex(0x7f631f1b79d0, FUTEX_WAIT, 12420, NULL) = -1 EAGAIN (Resource temporarily unavailable)

# sometime there is no call to futex() because the cloned process - the
# std::thread, has completed its execution.

# //////////////////////////
# read man clone, man futex
# //////////////////////////

set -e

CC=${CC-gcc}
CXX=${CXX-g++}
DBG=${DBG-gdb}

TEMPDIR=/tmp/sut

tearDown() {
    rm -rf ${TEMPDIR} /tmp/_ /tmp/_.* /tmp/__*
}

setUp() {
    tearDown
    mkdir -p ${TEMPDIR}
}

PROG=${TEMPDIR}/_
buildProgram() {
    cat >${TEMPDIR}/_.cpp <<"EOF"
#include <thread>
void compute() {
    static thread_local int arr[] = {0xDE, 0xAD, 0xBE, 0xEF};
    for (int i = 0; i < 1000; ++i) {
        *(arr + i % 4) += 1;
    }
}
int main() {
    std::thread first(compute);
    std::thread second(compute);
    first.join();
    second.join();
    return 0;
}
EOF
    ${CXX} ${TEMPDIR}/_.cpp -std=c++14 -o ${PROG} -lpthread
}

runProgram() {
    strace ${PROG} 2>&1 | \
    perl -ne '(/^clone\(/ || /^futex\(/) && print'
}

setUp
buildProgram
runProgram
tearDown