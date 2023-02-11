set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -save-temps")
add_executable(test_modern test.cpp)
target_link_libraries(test_modern jemalloc)