//
// Created by wein on 26/10/18.
//

// source
// master cxx mt L971

// POSIX semaphores, not part of the original pthread specification

// https://en.wikipedia.org/wiki/Semaphore_(programming)

// semaphores are simple integers, generally used as a resource
// count.
// to make them thread-safe, atomic operations (check and lock)
// are used

void RunTinyTests();

void test_nothing() {
    ;
}

int main(int argc, char **argv) {
    RunTinyTests();
    return 0;
}
