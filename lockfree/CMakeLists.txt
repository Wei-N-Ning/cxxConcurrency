
add_executable(lockfree_counter counter.cpp)
target_link_libraries(lockfree_counter PRIVATE doctest Threads::Threads)
add_test(NAME "lockfree::counter" COMMAND lockfree_counter)

add_executable(lockfree_stack stack.cpp)
target_link_libraries(lockfree_stack PRIVATE doctest Threads::Threads)
target_include_directories(lockfree_stack PRIVATE ${Boost_INCLUDE_DIRS})
add_test(NAME "lockfree::stack" COMMAND lockfree_stack)