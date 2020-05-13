
add_executable(stdthread_rand_per_thread rand_per_thread.cpp)
target_link_libraries(stdthread_rand_per_thread doctest)
add_test(NAME "stdthread::random-number-per-thread" COMMAND stdthread_rand_per_thread)

add_executable(stdthread_joinable joinable_thread.cpp)
target_link_libraries(stdthread_joinable PRIVATE doctest pthread)
add_test(NAME "stdthread::joinable" COMMAND stdthread_joinable)

add_executable(stdthread_pass_arguments pass_arguments.cpp)
target_link_libraries(stdthread_pass_arguments PRIVATE doctest pthread)
add_test(NAME "stdthread::pass-arguments" COMMAND stdthread_pass_arguments)