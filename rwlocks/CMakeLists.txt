
add_executable(readwritelocks main.cpp)
add_test(NAME "read-write-lock"
    COMMAND readwritelocks)


add_executable(readwritelocks_call_once call_once.cpp)
target_link_libraries(readwritelocks_call_once PRIVATE doctest Threads::Threads)
target_include_directories(readwritelocks_call_once PRIVATE ${Boost_INCLUDE_DIRS})
add_test(NAME "readwritelocks::call-once" COMMAND readwritelocks_call_once)