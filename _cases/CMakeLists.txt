
add_executable(toupper main.cpp)
set_target_properties(toupper
    PROPERTIES
    CXX_STANDARD 14)
target_link_libraries(toupper
    PUBLIC
    pthread)