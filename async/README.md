# Asynchronous Execution

## Future and Promise 

source: mastering c++ programming L5803

> if we deal with an asynchronous function, its return value is considered as
> something called a promise or a promise object. It's the way the system 
> notifies us when the asynchronous function has completed.
> the promise object has three states:
> Pending
> Rejected
> Fulfilled

## Future Pool

inspired by optimized C++ L7656 (prefer std::async to std::thread), 
I created a FuturePool using the same deque based mechanism, see: ../stdthread/thread_pool.cpp

> the template function std::async() runs a callable object in the context
> of a thread but the implementation is allowed to reuse threads
> the standard hints that std::async() maybe implemented using a thread pool
> on windows, std::async is significantl faster

see the comparison of in future_perf.cpp which covers:

- my incorrect impl of thread-pool and future-pool
- web found: <https://github.com/progschj/ThreadPool>
- boost::asio::thread-pool (recommend)
- creating as many async tasks as I want

the advantage is not very obvious (workload compute 128 fib numbers, each input being 40)

```text
weining@weining-ubuntu:build$ time ./async/async_future_perf thread-pool

real    0m4.538s
user    0m52.397s
sys     0m0.020s
weining@weining-ubuntu:build$ time ./async/async_future_perf boost-thread-pool

real    0m4.628s
user    0m53.736s
sys     0m0.012s
weining@weining-ubuntu:build$ time ./async/async_future_perf ThreadPool

real    0m4.604s
user    0m53.424s
sys     0m0.009s
```

the lock in the github version `ThreadPool` can be the cause of slow down
