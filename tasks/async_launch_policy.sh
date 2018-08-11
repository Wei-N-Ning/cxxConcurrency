#!/usr/bin/env bash

# EMC Item 35 P244

# you will want to pass std::launch::async to std::async
# that will ensure that the function you want to run really
# executes on a different thread

# Note compute_2() force the creation of a new thread
# compute_1() may not do that (it doesn't create
# new thread at the time of writing)

# Breakpoint 1, compute_1 () at /tmp/_.cpp:5
#5	bool compute_1() {
#
#Breakpoint 2, compute_2 () at /tmp/_.cpp:9
#9	bool compute_2() {
#[New Thread 0x7ffff6f4e700 (LWP 16208)]
#[Thread 0x7ffff6f4e700 (LWP 16208) exited]
#[Inferior 1 (process 16204) exited normally]

source ../utilities.sh

buildProgram() {
    cat > /tmp/_.cpp <<EOF
#include <future>
int factory() {
    return 11;
}
bool compute_1() {
    auto fut = std::async(factory);
    return fut.get() == 11;
}
bool compute_2() {
    auto fut = std::async(std::launch::async, factory);
    return fut.get() == 11;
}
int main() {
    if (compute_1() && compute_2()) {
        return 0;
    }
    return 1;
}
EOF
}

runProgram() {
    gdb -batch \
-ex "rbreak compute_" \
-ex "r" \
-ex "c" \
-ex "c" \
/tmp/_
}

setUp
buildProgram
compile g++ /tmp/_.cpp /tmp/_ -lpthread
runProgram
