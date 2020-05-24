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
    int b{};
    // in action 2nd P/162
    // If your UDT is the same size as (or smaller than) an int or a void*, most common
    //platforms will be able to use atomic instructions for std::atomic<UDT>. Some plat-
    //forms will also be able to use atomic instructions for user-defined types that are twice
    //the size of an int or void*.
    // HOWEVER, by increasing the size of Foo by 2 * sizeof(int), the resulting
    // atomic value will be defunct (can not compile)
    // int arr[2];

    static Foo create() {
        return Foo{};
    }

    static Foo create(int val1, int val2) {
        Foo f;
        f.a = val1;
        f.b = val2;
        return f;
    }
    // by defining a non-trivial copy-assignment op
    // I can no longer use Foo as an atomic value
//    Foo &operator=(const Foo &) {
//        return *this;
//    }
};

// in action 2nd P/161
// in order to use std::atomic<T> for some user-defined type T,
// this type must have a trivial copy-assignment operator
// P/162
// the compare-exchange operations do bitwise comparison as if using memcmp,
// rather than using any comparison operator that may be defined for T
//
TEST_CASE ("atomic struct") {
    std::atomic<Foo> f = Foo::create(1, 2);
    CHECK(f.is_lock_free());
    Foo f1 = f.load();
    Foo f2 = Foo::create(10, 20);
    f.compare_exchange_weak(f1, f2);
    CHECK_EQ(f.load().a, 10);
    CHECK_EQ(f.load().b, 20);
}

TEST_CASE ("atomic pointer") {
    using Item = std::tuple<int, int>;
    using PItem = std::shared_ptr<Item>;
    auto pi = std::make_shared<Item>(1, 2);

    // in action 2nd P/160
    std::atomic<Item *> atompi{pi.get()};
}

TEST_CASE ("atomic flag") {
    std::atomic_flag af = ATOMIC_FLAG_INIT;


}
