//
// Created by weining on 23/5/20.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"

#include <thread>
#include <memory>
#include <iostream>
#include <mutex>
#include "../stdthread/ThreadPool.hh"

// in action 2nd P/89
// bad pattern (the "double-check" pattern):
// if (!resource) {
//    acquire_scoped_lock;
//    if (!resource) {
//        initialize_resource;
//    }}
// good pattern: the call-once pattern as shown below
// note how to invoke the member function

// in action 2nd P/90
// static data member, since C++11, is initialized in exactly one thread
// (i.e. guaranteed to be thread-safe)
// this is equivalent to std::call_once

struct Resource {
    explicit Resource(int n) {
        using namespace std;
        cout << "resource ctor" << endl;
    }
};

class Worker {
public:
    void work() {
        // how to invoke non-static method:
        // https://stackoverflow.com/questions/23197333/why-is-this-pointer-needed-when-calling-stdcall-once
        // also see in action 2nd P/90
        std::call_once(resource_flag, &Worker::init_resource, this);
    }

private:
    std::shared_ptr<Resource> resource_ptr{nullptr};
    std::once_flag resource_flag;

    void init_resource() {
        resource_ptr.reset(new Resource{1});
    }
};

TEST_CASE ("") {
    ThreadPoolGithub::ThreadPool tp(std::thread::hardware_concurrency());
    Worker w{};
    for (auto i = 0; i < 128; ++i) {
        tp.enqueue([&w]() { w.work(); });
    }
}

