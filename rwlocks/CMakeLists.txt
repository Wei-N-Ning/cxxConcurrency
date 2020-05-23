
add_executable(readwritelocks main.cpp)
add_test(NAME "read-write-lock" COMMAND readwritelocks)

add_executable(readwritelocks_call_once call_once.cpp)
target_link_libraries(readwritelocks_call_once PRIVATE doctest Threads::Threads)
target_include_directories(readwritelocks_call_once PRIVATE ${Boost_INCLUDE_DIRS})
add_test(NAME "readwritelocks::call-once" COMMAND readwritelocks_call_once)

add_executable(readwritelocks_shared_mutex shared_mutex.cpp)
target_link_libraries(readwritelocks_shared_mutex PRIVATE doctest Threads::Threads)
target_include_directories(readwritelocks_shared_mutex PRIVATE ${Boost_INCLUDE_DIRS})
add_test(NAME "readwritelocks::shared-mutex" COMMAND readwritelocks_shared_mutex)

