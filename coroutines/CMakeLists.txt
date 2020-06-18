
add_executable(coroutines_boost boost_coroutine.cpp)
target_include_directories(coroutines_boost PRIVATE ${Boost_INCLUDE_DIRS})
# boost coroutines2 requires linking against libboost_context
target_link_libraries(coroutines_boost PRIVATE ${Boost_LIBRARIES})

