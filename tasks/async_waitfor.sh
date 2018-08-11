#!/usr/bin/env bash

# EMC++ Item 36, P247
# if callee is deferred, fut.wait_for will always return
# std::future_status::deferred

# P248
# wait_for() can also be used to detected the type of
# the task owned by a future; pass in 0s or 0ms

source ../utilities.sh

buildProgram() {
    cat > /tmp/_.cpp <<EOF
#include <chrono>
#include <future>
using namespace std::literals;
int compute() {
    return 0;
}
int main() {
    auto fut = std::async(std::launch::deferred, compute);
    if (std::future_status::deferred == fut.wait_for(0ms)) {
        return 0;
    }
    return 1;
}
EOF
}

setUp
buildProgram
compile g++ /tmp/_.cpp /tmp/_ -lpthread
/tmp/_
