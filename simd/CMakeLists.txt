add_executable(simd_demo main.cpp)
target_link_libraries(simd_demo benchmark)

set(CMAKE_CXX_FLAGS "-mavx2")
add_executable(simd_api_demo demo.cpp)