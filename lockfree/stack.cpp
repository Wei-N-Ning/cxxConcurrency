//
// Created by weining on 14/5/20.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"

#include <atomic>
#include <thread>
#include <boost/asio.hpp>

// expert C++ L6175
// lockfree stack with leaking issue
// shows the basic use of CAS idiom

struct Node {
    Node *next{nullptr};
};

// L6209
// we can instantiate std::atomic<> for almost any user-defined type, that is,
// std::atomic<T>,
// however you should note that the interface of std::atomic<T> is limited to
// the following functions:
// load()
// store()
// exchange()
// compare_exchange_weak()
// compare_exchange_strong()
// wait()
// notify_one()
// notify_all()
using ad = std::atomic<Node>;

// NOTE: not solving the memory leak issue
class Stack {
public:
    [[nodiscard]] std::size_t size() const {
        std::size_t sz{0};
        for (auto p = head.load(); p != nullptr; p = p->next, ++sz);
        return sz;
    }

    void push() {
        Node *new_elem = new Node{};
        new_elem->next = head.load();
        // []<--[]<--[]<--... <--[]
        //                       []<--[]
        //                                      expected val  desired val
        while (!head.compare_exchange_weak(new_elem->next, new_elem));
        //                              if head is new_elem->next, then
        //                                 head becomes new_elem
    }

    void pop() {
        Node *old_head = head.load();
        // []<--[]<--[]<--... <--[]<--[]
        //                    <--[]
        while (!head.compare_exchange_weak(old_head, old_head->next));
        // if head is old_head, then
        //    head becomes old_head->next
        // (old head is gone from the chain)
    }

private:
    std::atomic<Node *> head{nullptr};
};

int fib(int n) {
    return (n == 1 or n == 0) ? 1 : fib(n - 1) + fib(n - 2);
}

TEST_CASE ("") {
    Stack st;
        CHECK_EQ(st.size(), 0);
    std::size_t work_size = 128;

    {
        boost::asio::thread_pool tp(std::thread::hardware_concurrency());
        for (auto i = 0; i < work_size; ++i) {
            boost::asio::post(
                tp,
                [&st]() {
                    fib(30);
                    st.push();
                }
            );
        }
        tp.join();
    }
    CHECK_EQ(st.size(), work_size);

    {
        boost::asio::thread_pool tp(std::thread::hardware_concurrency());
        for (auto i = 0; i < work_size; ++i) {
            boost::asio::post(
                tp,
                [&st]() {
                    fib(31);
                    st.pop();
                }
            );
        }
        tp.join();
    }

     CHECK_EQ(st.size(), 0);
}
