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
