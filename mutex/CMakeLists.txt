
add_executable(comparisonToAtomic comparison_to_atomic.cpp)
set_target_properties(comparisonToAtomic
    PROPERTIES
    CXX_STANDARD 14)
target_link_libraries(comparisonToAtomic
    PUBLIC
    pthread)
add_test(NAME "mutex::comparison"
    COMMAND comparisonToAtomic)
