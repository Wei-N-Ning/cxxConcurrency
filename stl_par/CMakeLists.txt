add_executable(stl_par_reduce par_reduce.cpp)
set_target_properties(stl_par_reduce PROPERTIES CMAKE_CXX_STANDARD 17)
target_link_libraries(stl_par_reduce doctest)
add_test(NAME "STL::par::reduce" COMMAND stl_par_reduce)
