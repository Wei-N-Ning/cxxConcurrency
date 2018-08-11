
add_executable(future_wrap_func future_wrap_func.cpp)
set_target_properties(future_wrap_func
    PROPERTIES
    CXX_STANDARD 14)
target_link_libraries(future_wrap_func
    PUBLIC
    pthread)
add_test(NAME "future::wrap-one-func"
    COMMAND future_wrap_func)
