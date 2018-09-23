cmake_minimum_required(VERSION 3.10)
project(learncpp)

set(CMAKE_CXX_STANDARD 11)

add_executable(learncpp main.cpp)

add_subdirectory(smart_pointers)
add_subdirectory(ch13)

add_subdirectory(ch16)