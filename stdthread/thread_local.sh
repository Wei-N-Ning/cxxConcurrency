#!/usr/bin/env bash

# observe the addresses of the int array "arr"
# even though arr has static storage, its address
# is unique to each thread

source ../utilities.sh

buildProgram() {
    cat > /tmp/_.cpp <<EOF
#include <thread>
void compute() {
    static thread_local int arr[] = {0xDE, 0xAD, 0xBE, 0xEF};
    arr[0]++;
}
int main() {
    std::thread first(compute);
    std::thread second(compute);
    for (int i = 0; i < 50; ++i) {
        ;
    }
    first.join();
    second.join();
    return 0;
}
EOF
}

printArrAddress() {
    gdb -batch \
-ex "b _.cpp:4" \
-ex "r" \
-ex "print (void *)arr" \
-ex "cont" \
-ex "print (void *)arr" \
-ex "cont" \
/tmp/_
}

setUp
buildProgram
compile g++ /tmp/_.cpp /tmp/_ -lpthread
printArrAddress
