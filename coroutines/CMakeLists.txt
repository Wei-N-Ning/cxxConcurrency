
add_executable(coroutines_boost boost_coroutine.cpp)
target_include_directories(coroutines_boost PRIVATE ${Boost_INCLUDE_DIRS})

