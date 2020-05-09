add_executable(info_num_cores num_cores.cpp)
target_link_libraries(info_num_cores PRIVATE doctest)
add_test(NAME "info::num-cores" COMMAND info_num_cores)