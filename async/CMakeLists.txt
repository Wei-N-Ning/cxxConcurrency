
add_executable(async_future_perf future_perf.cpp)
target_link_libraries(async_future_perf PRIVATE Threads::Threads)
target_include_directories(async_future_perf PRIVATE ${Boost_INCLUDE_DIRS})
add_test(NAME "async::future-perf-test" COMMAND async_future_perf)