//
// Created by wein on 6/10/18.
//

// source:
// EMC++ Item 39 P266
// "one-shot" event communication
// see also:
// how to pass parameter to threaded worker by reference
// http://en.cppreference.com/w/cpp/thread/thread/thread

#include <iostream>
#include <future>
#include <thread>

void worker(std::future<void>& fut, int workload[2]) {
    fut.wait();
    int sum = 0;
    for (int i = 0; i < 0xFFFF; ++i) {
        sum += 1;
    }
    workload[1] = sum;
}

void compute() {
    int workload[2];
    std::promise<void> p;
    auto fut = p.get_future();
    std::thread th(worker, std::ref(fut), std::ref(workload));
    int sum = 0;
    for (int i = 0; i < 0xFFFFF; ++i) {
        sum += 1;
    }
    p.set_value();
    workload[0] = sum;
    th.join();
    std::cout << workload[0] << ", " << workload[1] << std::endl;
}

int main() {
    compute();
    return 0;
}
