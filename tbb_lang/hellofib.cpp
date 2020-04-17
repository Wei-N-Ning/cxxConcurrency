//
// Created by weining on 17/4/20.
//

#include <iostream>
#include <vector>
#include <string>
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"

using namespace tbb;
using namespace std;
static const size_t N = 23;

int fib(int n) {
    if (n == 0 || n == 1) {
        return 1;
    }
    return fib(n - 1) + fib(n - 2);
}

int main() {

    string str[N] = { string("a"), string("b") };
    for (size_t i = 2; i < N; ++i)
        str[i] = str[i-1]+str[i-2];

    string &to_scan = str[N-1];

    size_t num_elem = to_scan.size();

    size_t *max = new size_t[num_elem];
    size_t *pos = new size_t[num_elem];

    parallel_for(blocked_range<size_t>(0, num_elem ),
                 [](const auto &n) {
                     for (const auto &n : vector{26, 27, 28, 29, 30, 31}) {
                         fib(n);
                     }
                 });

    delete[] pos;
    delete[] max;
    return 0;
}