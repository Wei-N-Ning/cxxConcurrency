
add_executable(async_future_perf future_perf.cpp)
target_link_libraries(async_future_perf PRIVATE Threads::Threads)
target_include_directories(async_future_perf PRIVATE ${Boost_INCLUDE_DIRS})

add_executable(async_self_updating_pool self_updating_pool.cpp)
target_link_libraries(async_self_updating_pool PRIVATE Threads::Threads)
add_test(NAME "async::self_updating_pool" COMMAND async_self_updating_pool)