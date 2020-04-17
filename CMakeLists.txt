cmake_minimum_required(VERSION 3.8)
project(C++ConcurrencyExamples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
add_subdirectory(vendor/doctest)

set(CMAKE_BUILD_TYPE Release)

set(Boost_NO_SYSTEM_PATHS TRUE)
set(Boost_USE_STATIC_LIBS TRUE)
set(BOOST_ROOT ${PROJECT_SOURCE_DIR}/vendor/boost_1.72.0)
find_package(Boost 1.72.0 REQUIRED COMPONENTS regex)

add_subdirectory(tasks)
add_subdirectory(mutex)
add_subdirectory(rwlocks)
add_subdirectory(_cases)
add_subdirectory(stdthread)
if (UNIX)
    # true for apple and linux
    add_subdirectory(pthread)
endif()
add_subdirectory(communication)
