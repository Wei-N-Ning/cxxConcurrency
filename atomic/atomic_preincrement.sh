#!/usr/bin/env bash

# source:
# EMC++ Item 40 P273

setUp() {
    set -e
}

buildSUT() {
    cat > /tmp/_.cc <<EOF
#include <atomic>
void sut() {
    std::atomic<int> a{0xdead};
    (*(&a))++;  // atomic incr
    int b{0xbeef};
    (*(&b))++;  // normal incr
}
int main() {
    sut();
    return 0;
}
EOF
    g++ -std=c++11 -g -Wall -Werror \
-fno-stack-check -fno-stack-protector -fomit-frame-pointer \
-o /tmp/_ /tmp/_.cc
}

disassemble() {
    local func=${2-"main"}
    gdb -batch \
    -ex "set disassembly-flavor intel" \
    -ex "file ${1:?missing binary path}" \
    -ex "disassemble /rs ${func}"
}

compareIncr() {
    disassemble /tmp/_ sut | awk '
/atomic incr/ {
    for (i = 1; i < 7; i++) {
        print
        getline
    }
}
/normal incr/ {
    for (i = 1; i < 6; i++) {
        print
        getline
    }
}'
}

setUp
buildSUT
compareIncr
