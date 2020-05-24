
add_executable(atomic_type atomic_type.cpp)
target_link_libraries(atomic_type PRIVATE doctest)
add_test(NAME "atomic::type" COMMAND atomic_type)