cmake_minimum_required(VERSION 3.8)
project(C++ConcurrencyExamples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_subdirectory(vendor/doctest)
set(CMAKE_BUILD_TYPE Release)
set(BOOST_ROOT ${PROJECT_SOURCE_DIR}/vendor/boost-1.72.0)
set(HPX_ROOT ${PROJECT_SOURCE_DIR}/vendor/hpx-1.4.1)
set(HPX_DIR ${PROJECT_SOURCE_DIR}/vendor/hpx-1.4.1/lib/cmake/HPX)
find_package(HPX REQUIRED)

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
add_subdirectory(hpx_lang)