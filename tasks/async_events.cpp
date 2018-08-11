//
// Created by wein on 6/10/18.
//

#include <iostream>
#include <atomic>
#include <thread>
#include <condition_variable>

static std::atomic<bool> flag;
static std::condition_variable cv;
static std::mutex m;
static int workload_1[2];
static int workload_2[2];

void worker_1() {
    while (!flag);
    uint sum = 0;
    for (uint i = 0; i < 0xFFFF; ++i) {
        sum += 1;
    }
    if (sum) {
        workload_1[1] = sum;
    }
}

void compute_1() {
    std::thread th(worker_1);
    int sum = 0;
    for (int i = 0; i < 0xFFFFF; ++i) {
        sum += 1;
    }
    flag.store(true);
    workload_1[0] = sum;
    th.join();
}

void worker_2() {
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk);
    int sum = 0;
    for (int i = 0; i < 0xFFFF; ++i) {
        sum += 1;
    }
    if (sum) {
        workload_2[1] = sum;
    }
}

void compute_2() {
    std::thread th(worker_2);
    int sum = 0;
    for (int i = 0; i < 0xFFFFF; ++i) {
        sum += 1;
    }
    cv.notify_one();
    workload_2[0] = sum;
    th.join();
}

int main(int argc, char** argv) {
    if (argc == 1) {
        compute_1();
        std::cout << workload_1[0] << ", " << workload_1[1] << std::endl;
    } else {
        compute_2();
        std::cout << workload_2[0] << ", " << workload_2[1] << std::endl;
    }
    return 0;
}