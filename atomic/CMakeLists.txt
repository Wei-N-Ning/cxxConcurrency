
add_executable(atomic_type atomic_type.cpp)
target_link_libraries(atomic_type PRIVATE doctest Threads::Threads)
add_test(NAME "atomic::type" COMMAND atomic_type)

add_executable(atomic_compare_exchange compare_exchange.cpp)
target_link_libraries(atomic_compare_exchange PRIVATE doctest Threads::Threads)
add_test(NAME "atomic::compare-exchange" COMMAND atomic_compare_exchange)

add_executable(atomic_shared_ptr shared_ptr.cpp)
target_link_libraries(atomic_shared_ptr PRIVATE doctest Threads::Threads)
add_test(NAME "atomic::shared-ptr" COMMAND atomic_shared_ptr)