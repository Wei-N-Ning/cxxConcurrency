#!/usr/bin/env bash

# EMC++ Item 35 P243

# the factory produces a return value;
# call the future.get() method

# auto-deduced future's type is std::future<A>

source ../utilities.sh

buildProgram() {
    cat > /tmp/_.cpp <<EOF
#include <future>
class A {
public:
    int value = 11;
};
template<typename T>
class TypeTester;
A factory() {
    return A();
}
int main() {
    auto future = std::async(factory);
    auto a = future.get();
    if (a.value > 5) {
        ;
    }
    return 0;
}
EOF
}

setUp
buildProgram
compile g++ /tmp/_.cpp /tmp/_ -lpthread
disassemble /tmp/_ main
