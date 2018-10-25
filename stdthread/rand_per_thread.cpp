//
// Created by wein on 25/10/18.
//
// source:
// master cxx mt L387

// mt19937:
// http://www.cplusplus.com/reference/random/mt19937/


#include <iostream>
#include <random>
#include <thread>

int randGen(int min_, int max_) {
    using namespace std;

    // every thread will create its own generator
    // a hash of the internal thread identifier is used as a seed for
    // the generator
    static thread_local mt19937 generator(
        hash<thread::id>()(
            this_thread::get_id()
        )
    );
    uniform_int_distribution<int> distribution(min_, max_);
    return distribution(generator);
}

void RunTinyTests();

void test_randGen() {
    // note how the number remains the same between executions
    // (main thread only)
    std::cout
    << randGen(1, 0x1111111)
    << randGen(1, 0x1111111)
    << randGen(1, 0x1111111)
    << std::endl;
}

int main() {
    RunTinyTests();
    return 0;
}