#!/usr/bin/env bash

# source:
# EMC++ Item 40 P276

# "volatile is the way we tell compilers that we are
# dealing with special memory. Its meaning to compilers
# is Don't perform any optimizations on operations on
# this memory."

# NOTE, volatile keyword is part of type: volatile int
# similarly there are:
# volatile int&
# volatile int*

# see how volatile keyword help sut1 subroutine
# to keep the variable and for-loop relatively
# unchanged even under -O3 mode
# sut2 subroutine on the contrary is completely
# changed to nothing

source ./utilities.sh

buildSUT() {
    cat > /tmp/_.cc <<EOF
void sut1(int& o_value) {
    volatile int a{1000};
    for (int i = 0; i < a; ++i) {
        o_value += i;
    }
}
void sut2(int& o_value) {
    int a{1000};
    for (int i = 0; i < a; ++i) {
        o_value += i;
    }
}
int main() {
    int a{0};
    int b{0};
    sut1(a);
    sut2(b);
    return 0;
}
EOF
}

setUp
buildSUT
compile "g++ -O3" /tmp/_.cc
disassemble /tmp/_ sut1
disassemble /tmp/_ sut2

