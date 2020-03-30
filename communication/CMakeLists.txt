
add_executable(comm_scheduler scheduler.cpp)
set_target_properties(comm_scheduler
    PROPERTIES
    CXX_STANDARD 14)
add_test(NAME
    "communication::scheduler"
    COMMAND
    comm_scheduler)
