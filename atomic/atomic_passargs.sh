#!/usr/bin/env bash

# this is to observe the non-atomic portion (passing
# arguments into the atomic method)

source  ./utilities.sh

buildSUT() {
    cat > /tmp/_.cc <<EOF
#include <atomic>

int main() {
    std::atomic<int> ai{3};
    int  tst_val = 0xDE;
    int  new_val = 0xAD;
    bool exchanged = ai.compare_exchange_strong( tst_val, new_val );
    if (exchanged) {
        ;
    }
    return 0;
}
EOF
}

setUp
buildSUT
compile g++ /tmp/_.cc
disassemble /tmp/_ main
