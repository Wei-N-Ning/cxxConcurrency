//
// Created by weining on 14/5/20.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"

#include <atomic>
#include <thread>
#include <memory>
#include <boost/asio.hpp>

// expert C++ L6175
// lockfree stack with leaking issue
// shows the basic use of CAS idiom

struct Node {
    std::shared_ptr<Node> next{nullptr};
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

// UPDATE:
// after reading: in action 2nd P/164, I realize I can resolve the memory leak issue
// caused by using raw pointer - I need to switch to using shared_ptr and taking
// advantage of the atomic free function STL provides for std::shared_ptr,
// see:
// https://en.cppreference.com/w/cpp/atomic/atomic_compare_exchange
// note that the expected value must be of a pointer type
// the "atomic shared ptr" must also be passed to the free function as pointer
class Stack {
public:
    [[nodiscard]] std::size_t size() const {
        std::size_t sz{0};
        for (auto p = std::atomic_load(&head); p != nullptr; p = p->next, ++sz);
        return sz;
    }

    void push() {
        auto new_elem = std::make_shared<Node>();
        new_elem->next = std::atomic_load(&head);
        // []<--[]<--[]<--... <--[]
        //                       []<--[]
        //                                      expected val  desired val
        while (!std::atomic_compare_exchange_weak(&head, &new_elem->next, new_elem));
        //                              if head is new_elem->next, then
        //                                 head becomes new_elem
    }

    void pop() {
        auto old_head = std::atomic_load(&head);
        // []<--[]<--[]<--... <--[]<--[]
        //                    <--[]
        while (!std::atomic_compare_exchange_weak(&head, &old_head, old_head->next));
        // if head is old_head, then
        //    head becomes old_head->next
        // (old head is gone from the chain)
    }

private:
    std::shared_ptr<Node> head{nullptr};
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
