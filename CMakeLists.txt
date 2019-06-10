cmake_minimum_required(VERSION 3.10)
project(learncpp)

set(CMAKE_CXX_STANDARD 14)

add_executable(learncpp main.cpp)

add_subdirectory(smart_pointers)
add_subdirectory(ch13)

add_subdirectory(ch16)
add_subdirectory(concurrency)
add_subdirectory(system)
add_subdirectory(concurrent_queue)

add_subdirectory(ch9)
add_subdirectory(ch10)
add_subdirectory(ch15)
add_subdirectory(object_model)

add_subdirectory(uv_wrapper)
add_subdirectory(String)