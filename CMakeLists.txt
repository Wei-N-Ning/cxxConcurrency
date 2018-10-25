cmake_minimum_required(VERSION 3.8)
project(C++ConcurrencyExamples)

enable_testing()

add_subdirectory(tasks)
add_subdirectory(mutex)
add_subdirectory(rwlocks)
add_subdirectory(_cases)

