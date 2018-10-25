
add_tinytest_executable(rand_per_thread
    rand_per_thread.cpp)
set_target_properties(rand_per_thread
    PROPERTIES
    CXX_STANDARD 14)
add_test(NAME
    "stdthread::random-number-per-thread"
    COMMAND
    rand_per_thread)
