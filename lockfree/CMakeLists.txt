
add_executable(lockfree_counter counter.cpp)
target_link_libraries(lockfree_counter PRIVATE doctest Threads::Threads)
add_test(NAME "lockfree::counter" COMMAND lockfree_counter)

add_executable(lockfree_stack stack.cpp)
target_link_libraries(lockfree_stack PRIVATE doctest Threads::Threads)
target_include_directories(lockfree_stack PRIVATE ${Boost_INCLUDE_DIRS})
add_test(NAME "lockfree::stack" COMMAND lockfree_stack)

add_executable(lockfree_parallel_cells parallel_cells.cpp)
target_link_libraries(lockfree_parallel_cells PRIVATE doctest Threads::Threads)
target_include_directories(lockfree_parallel_cells PRIVATE ${Boost_INCLUDE_DIRS})
add_test(NAME "lockfree::parallel-cells" COMMAND lockfree_parallel_cells)

add_executable(lockfree_experimental_atomic_shared_ptr experimental_atomic_shared_ptr.cpp)
target_link_libraries(lockfree_experimental_atomic_shared_ptr PRIVATE doctest Threads::Threads)
add_test(NAME "lockfree::experimental-atomic-shared-ptr" COMMAND lockfree_experimental_atomic_shared_ptr)