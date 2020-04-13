
add_executable(future_wrap_func future_wrap_func.cpp)
target_link_libraries(future_wrap_func PUBLIC pthread)
add_test(NAME "future::wrap-one-func"
    COMMAND future_wrap_func)
