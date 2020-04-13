
add_executable(readwritelocks main.cpp)
add_test(NAME "read-write-lock"
    COMMAND readwritelocks)


