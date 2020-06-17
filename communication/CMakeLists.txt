
add_executable(comm_scheduler scheduler.cpp)
add_test(NAME "communication::scheduler" COMMAND comm_scheduler)

# boost::interprocess dependes on:
# pthread (for synchronization)
# rt (for shm_open)
# source of this recipe: https://github.com/google/benchmark/issues/338
# find_library(): https://cmake.org/cmake/help/latest/command/find_library.html
find_library(LIB_RT rt REQUIRED)
add_executable(comm_boost_inter_process boost_inter_process.cpp)
target_include_directories(comm_boost_inter_process PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(comm_boost_inter_process PRIVATE Threads::Threads ${LIB_RT})
