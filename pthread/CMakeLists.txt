
add_executable(pthread_creation creation.cpp)
target_link_libraries(pthread_creation PUBLIC Threads::Threads doctest)
add_test(NAME
    "pthread::creation"
    COMMAND
    pthread_creation)

add_executable(pthread_mutex mutexing.cpp)
target_link_libraries(pthread_mutex PUBLIC Threads::Threads doctest)
add_test(NAME
    "pthread::mutexing"
    COMMAND
    pthread_mutex)

add_executable(pthread_rwlocks rwlocks.cpp)
target_link_libraries(pthread_rwlocks PUBLIC Threads::Threads doctest)
add_test(NAME
    "pthread::read-write-locks"
    COMMAND
    pthread_rwlocks)

add_executable(pthread_barriers barriers.cpp)
target_link_libraries(pthread_barriers PUBLIC Threads::Threads doctest)
add_test(NAME
    "pthread::barriers"
    COMMAND
    pthread_barriers)

add_executable(pthread_semaphores semaphores.cpp)
target_link_libraries(pthread_semaphores PUBLIC Threads::Threads doctest)
add_test(NAME
    "pthread::semaphores"
    COMMAND
    pthread_semaphores)