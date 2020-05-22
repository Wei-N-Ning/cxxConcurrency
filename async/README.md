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

see the comparison of: 

- my (incorrect) thread pool
- my (also incorrect) future pool
- boost asio thread pool (but it also creates as many pthreads as there are jobs???!!!)
- as many futures as it needs

the advantage is not very obvious (workload compute 128 fib numbers, each input being 40)
