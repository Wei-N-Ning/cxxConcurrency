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

template<typename T>
struct Node {
    T data;

    explicit Node(const T &val) : data{val} {}

    explicit Node(T &&val) : data{val} {}

    Node(const Node &) = default;

    Node(Node &&) noexcept = default;

    Node &operator=(const Node &) = default;

    Node &operator=(Node &&) noexcept = default;

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
template<typename T>
class Stack {
public:
    [[nodiscard]] std::size_t size() const {
        std::size_t sz{0};
        for (auto p = std::atomic_load(&head); p != nullptr; p = p->next, ++sz);
        return sz;
    }

    void push(const T &val) {
        // in action 2nd P/234
        // the gist of lockfree push()
        // if it (compare_exchange_weak()) returns false to indicate that the
        // comparison failed (for example, because head was modified by another
        // thread), the value supplied as the first parameter (new_node->next)
        // is updated to the current value of head. You therefore don't have to
        //  reload head each time through the loop, because the compiler does
        //  that for you.
        // Also, because you’re looping directly on failure, you can use
        // compare_exchange_weak, which can result in more optimal code than
        // compare_exchange_strong on some architectures
        auto new_elem = std::make_shared<Node<T>>(val);
        new_elem->next = std::atomic_load(&head);
        // []<--[]<--[]<--... <--[]
        //                       []<--[]
        //                                      expected val  desired val
        while (!std::atomic_compare_exchange_weak(&head, &new_elem->next, new_elem));
        //                              if head is new_elem->next, then
        //                                 head becomes new_elem
    }

    void pop(T &val) {
        auto old_head = std::atomic_load(&head);
        // []<--[]<--[]<--... <--[]<--[]
        //                    <--[]
        while (old_head &&
            !std::atomic_compare_exchange_weak(&head, &old_head, old_head->next));
        // if head is old_head, then
        //    head becomes old_head->next
        // (old head is gone from the chain)
        val = old_head->data;
    }

private:
    std::shared_ptr<Node<T>> head{nullptr};
};

int fib(int n) {
    return (n == 1 or n == 0) ? 1 : fib(n - 1) + fib(n - 2);
}

TEST_CASE ("is atomic shared_ptr lock-free??") {
    // this shows that atomic_load, when given a shared_ptr, uses lock
    // therefore it has perf impact
    // see: in action 2nd P/249
    using PNode = std::shared_ptr<Node<int>>;
    PNode pNode{nullptr};
    CHECK_FALSE(std::atomic_is_lock_free(&pNode));
}

TEST_CASE ("push then pop") {
    Stack<int> st;
    CHECK_EQ(st.size(), 0);

    std::size_t work_size = 128;

    {
        boost::asio::thread_pool tp(std::thread::hardware_concurrency());
        for (auto i = 0; i < work_size; ++i) {
            boost::asio::post(
                tp,
                [&st, val = i]() {
                    fib(20);
                    st.push(val);
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
                [&st, val = i]() mutable {
                    fib(21);
                    st.pop(val);
                }
            );
        }
        tp.join();
    }

    CHECK_EQ(st.size(), 0);
}

//TEST_CASE ("push and pop at the same time") {
//    Stack<int> st;
//    CHECK_EQ(st.size(), 0);
//    std::size_t work_size = 256;
//    {
//        boost::asio::thread_pool tp(std::thread::hardware_concurrency());
//        for (auto i = 0; i < work_size; ++i) {
//            boost::asio::post(
//                tp,
//                [&st, val = i]() {
//                    auto o = fib(30);
//                    if (val % 2 == 0) {
//                        st.push(o);
//                    } else {
//                        st.pop(o);
//                    }
//                }
//            );
//        }
//        tp.join();
//    }
//}
