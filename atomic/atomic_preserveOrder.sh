#!/usr/bin/env bash

source ./utilities.sh

buildSUT() {
    cat > /tmp/_.cc <<EOF
#include <atomic>
void process(int times) {
    int sum = 0;
    for (int i = 0; i < times; ++i) {
        sum += i;
    }
}
int main() {
    std::atomic<bool> dirty{true};
    process(1000);
    dirty = false;
    return 0;
}
EOF
}

setUp
buildSUT
compile "g++" /tmp/_.cc
disassemble /tmp/_ main
