cmake_minimum_required(VERSION 3.8)

set(CMAKE_C_COMPILER gcc-9)
set(CMAKE_CXX_COMPILER g++-9)

project(C++ConcurrencyExamples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
add_subdirectory(vendor/doctest)

set(CMAKE_BUILD_TYPE Release)

set(Boost_NO_SYSTEM_PATHS TRUE)
set(Boost_USE_STATIC_LIBS TRUE)
set(BOOST_ROOT ${PROJECT_SOURCE_DIR}/vendor/boost-1.72.0)
find_package(Boost 1.72.0 REQUIRED COMPONENTS regex context)

# this is recommended way to bring in and link to the platform specific thread library
find_package(Threads)

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
add_subdirectory(get_info)
add_subdirectory(lockfree)
add_subdirectory(async)
add_subdirectory(synchronize)
add_subdirectory(atomic)
add_subdirectory(coroutines)
add_subdirectory(actors)
