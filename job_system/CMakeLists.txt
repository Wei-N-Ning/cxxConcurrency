
add_executable(local_task_queue local_task_queue.cpp)
target_link_libraries(local_task_queue PRIVATE Threads::Threads autotimer)

add_executable(mutex_to_queue mutex_to_queue.cpp)
target_link_libraries(mutex_to_queue PRIVATE Threads::Threads autotimer)
