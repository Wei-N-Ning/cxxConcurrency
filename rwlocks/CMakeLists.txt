
add_executable(readwritelocks main.cpp)
set_target_properties(readwritelocks
    PROPERTIES
    CXX_STANDARD 14)
add_test(NAME "read-write-lock"
    COMMAND readwritelocks)


