
add_executable(comparisonToAtomic comparison_to_atomic.cpp)
set_target_properties(comparisonToAtomic
    PROPERTIES
    CXX_STANDARD 14)
target_link_libraries(comparisonToAtomic
    PUBLIC
    pthread)
add_test(NAME "mutex::comparison"
    COMMAND comparisonToAtomic)

add_executable(mutex_efficient_locking efficient_locking.cpp)
target_link_libraries(mutex_efficient_locking PRIVATE doctest)
add_test(NAME "mutex::efficient_locking_statement" COMMAND mutex_efficient_locking)