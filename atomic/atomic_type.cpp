//
// Created by weining on 24/5/20.
//
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"

#include <atomic>
#include <utility>
#include <memory>

struct Foo {
    int a{};
};

TEST_CASE ("atomic struct") {
    std::atomic<Foo> f;
    CHECK(f.is_lock_free());
    Foo f2;

    f.compare_exchange_weak(f2, f2);
}

TEST_CASE ("atomic pointer") {
    using Item = std::tuple<int, int>;
    using PItem = std::shared_ptr<Item>;
    auto pi = std::make_shared<Item>(1, 2);
    std::atomic<Item *> atompi{pi.get()};
}

TEST_CASE ("atomic flag") {
    std::atomic_flag af = ATOMIC_FLAG_INIT;


}