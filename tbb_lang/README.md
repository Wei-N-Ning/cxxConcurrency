# TBB

## DevOps: Build and integrate TBB

source: 

cpp-taskflow uses tbb and provides an example of how to integrate it via CMake:

<https://github.com/cpp-taskflow/cpp-taskflow/blob/master/CMakeLists.txt>

the example foo.cpp is a modified version of the tbb official helloworld, find sub string:

<https://software.intel.com/en-us/node/505530>

here is the earliest release note of the Cmake integration feature from the tbb developers

<https://github.com/oneapi-src/oneTBB/tree/tbb_2017/cmake>

### tbb repo, DON'T build from repo

clone from this offical repo: <https://github.com/oneapi-src/oneTBB>

**BEWARE**: `make -j12 all` will build and RUN ALL the targets in parallel
including the stress tests (which use xwindow). It can take a very
long time to finish. In short, **do not build** from the repo.

### Integration via CMake

here is a minimal example:

cmakelist

```cmake
cmake_minimum_required(VERSION 3.10)
project(test LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TBB_DIR /home/weining/work/tmp/oneTBB)
include(${TBB_DIR}/cmake/TBBBuild.cmake)

# Build Intel TBB with enabled Community Preview Features (CPF).
tbb_build(TBB_ROOT ${TBB_DIR} CONFIG_DIR TBB_DIR MAKE_ARGS tbb_cpf=1)

find_package(TBB REQUIRED tbb_preview)

# Link Intel TBB imported targets to the executable;
# "TBB::tbb_preview" can be used instead of "${TBB_IMPORTED_TARGETS}".
add_executable(foo foo.cpp)
target_link_libraries(foo ${TBB_IMPORTED_TARGETS})
```

source `foo.cpp`

```c++
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
```

the example shows that tbb efficiently maximizes the CPU usage.
