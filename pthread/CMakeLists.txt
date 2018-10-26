
add_tinytest_executable(pthread_creation creation.cpp)
set_target_properties(pthread_creation
    PROPERTIES
    CXX_STANDARD 14)
target_link_libraries(pthread_creation
    PUBLIC
    pthread)
add_test(NAME
    "pthread::creation"
    COMMAND
    pthread_creation)

add_tinytest_executable(pthread_mutex mutexing.cpp)
set_target_properties(pthread_mutex
    PROPERTIES
    CXX_STANDARD 14)
target_link_libraries(pthread_mutex
    PUBLIC
    pthread)
add_test(NAME
    "pthread::mutexing"
    COMMAND
    pthread_mutex)

add_tinytest_executable(pthread_rwlocks rwlocks.cpp)
set_target_properties(pthread_rwlocks
    PROPERTIES
    CXX_STANDARD 14)
target_link_libraries(pthread_rwlocks
    PUBLIC
    pthread)
add_test(NAME
    "pthread::read-write-locks"
    COMMAND
    pthread_rwlocks)

add_tinytest_executable(pthread_barriers barriers.cpp)
set_target_properties(pthread_barriers
    PROPERTIES
    CXX_STANDARD 14)
target_link_libraries(pthread_barriers
    PUBLIC
    pthread)
add_test(NAME
    "pthread::barriers"
    COMMAND
    pthread_barriers)

add_tinytest_executable(pthread_semaphores semaphores.cpp)
set_target_properties(pthread_semaphores
    PROPERTIES
    CXX_STANDARD 14)
target_link_libraries(pthread_semaphores
    PUBLIC
    pthread)
add_test(NAME
    "pthread::semaphores"
    COMMAND
    pthread_semaphores)