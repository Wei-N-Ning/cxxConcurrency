
add_executable(comm_scheduler scheduler.cpp)
add_test(NAME "communication::scheduler" COMMAND comm_scheduler)

# boost::interprocess dependes on:
# pthread (for synchronization)
# rt (for shm_open)
# source of this recipe: https://github.com/google/benchmark/issues/338
# find_library(): https://cmake.org/cmake/help/latest/command/find_library.html
find_library(LIB_RT rt REQUIRED)
add_executable(comm_boost_interprocess_atomic boost_interprocess_atomic.cpp)
target_include_directories(comm_boost_interprocess_atomic PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(comm_boost_interprocess_atomic PRIVATE Threads::Threads ${LIB_RT})

add_executable(comm_boost_interprocess_deque boost_interprocess_deque.cpp)
target_include_directories(comm_boost_interprocess_deque PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(comm_boost_interprocess_deque PRIVATE Threads::Threads ${LIB_RT})
