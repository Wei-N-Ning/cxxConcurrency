
add_executable(rand_per_thread
    rand_per_thread.cpp)
target_link_libraries(rand_per_thread doctest)
add_test(NAME
    "stdthread::random-number-per-thread"
    COMMAND
    rand_per_thread)
