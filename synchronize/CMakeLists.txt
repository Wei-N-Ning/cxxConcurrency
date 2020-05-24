
add_executable(synchronize_shared_future shared_future.cpp)
target_link_libraries(synchronize_shared_future PRIVATE doctest Threads::Threads)
add_test(NAME "synchronize::shared-future" COMMAND synchronize_shared_future)

