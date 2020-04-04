add_executable(stl_par_reduce par_reduce.cpp)
target_link_libraries(stl_par_reduce doctest)
add_test(NAME "STL::par::reduce" COMMAND stl_par_reduce)
