#!/usr/bin/env bash

# EMC++ Item 40 P278

source ./utilities.sh

buildProgram() {
    cat > /tmp/_.cpp <<EOF
#include <atomic>
int main() {
    int arr[] = {0xDE, 0xAD, 0xBE, 0xEF};
    std::atomic<int> a{arr[3]};
    arr[0]++;
    a.store(arr[0]);
    return 0;
}
EOF
}

setUp
buildProgram
compile g++ /tmp/_.cpp
disassemble /tmp/_
