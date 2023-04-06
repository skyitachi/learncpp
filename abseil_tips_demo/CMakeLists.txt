set(CMAKE_CXX_STANDARD 20)

#add_subdirectory("${CMAKE_SOURCE_DIR}/thirdparty/abseil-cpp")

add_executable(abseil_demo main.cpp)
#find_package(absl REQUIRED)

target_link_libraries(abseil_demo absl::strings)