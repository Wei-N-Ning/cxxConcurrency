cmake_minimum_required(VERSION 3.8)
project(C++ConcurrencyExamples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
include(tinyCUnit/tinyCUnit.cmake)

add_library(doctest INTERFACE)
target_include_directories(doctest INTERFACE doctest)

add_subdirectory(tasks)
add_subdirectory(mutex)
add_subdirectory(rwlocks)
add_subdirectory(_cases)
add_subdirectory(stdthread)
add_subdirectory(stl_par)
if (UNIX)
    # true for apple and linux
    add_subdirectory(pthread)
endif()
add_subdirectory(communication)