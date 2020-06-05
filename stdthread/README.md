# std::thread

## Scoped thread

in action 2nd: P/51

see: scoped_thread

## Thread Pool

NOTE: my own implementation is WRONG - it still creates as many threads as there are jobs!!!

take a look at boost's implementation:

<https://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/reference/thread_pool.html>

```c++
#include <boost/asio.hpp>
// ...
boost::asio::thread_pool tp(std::thread::hardware_concurrency());
```
