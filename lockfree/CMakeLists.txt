
add_executable(lockfree_counter counter.cpp)
target_link_libraries(lockfree_counter PRIVATE doctest pthread)
add_test(NAME "lockfree::counter" COMMAND lockfree_counter)

add_executable(lockfree_stack stack.cpp)
target_link_libraries(lockfree_stack PRIVATE doctest pthread)
add_test(NAME "lockfree::stack" COMMAND lockfree_stack)