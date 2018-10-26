cmake_minimum_required(VERSION 3.8)
project(C++ConcurrencyExamples)

enable_testing()
include(tinyCUnit/tinyCUnit.cmake)

add_subdirectory(tasks)
add_subdirectory(mutex)
add_subdirectory(rwlocks)
add_subdirectory(_cases)
add_subdirectory(stdthread)
if (UNIX)
    # true for apple and linux
    add_subdirectory(pthread)
endif()
