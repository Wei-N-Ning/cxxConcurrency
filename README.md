# C++ Concurrency and Threading

## future and thread pool

C++ 17 (GCC 7.5.0) does not ship threadpool.

see perf analysis: <https://stackoverflow.com/questions/14351352/does-asynclaunchasync-in-c11-make-thread-pools-obsolete-for-avoiding-expen>

here is a high-rating threadpool: <https://github.com/progschj/ThreadPool>

## Concurrent data structures

## DevOps

### use Threads::Threads instead of pthread for cross-platform portability

see the top level CMakeLists 

find_package(Threads) brings in a new target called `Threads::Threads`, which is
resolved to pthread for POSIX platforms
