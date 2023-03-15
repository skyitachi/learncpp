add_executable(abseil_demo main.cpp)
find_package(absl REQUIRED)

target_link_libraries(abseil_demo absl::strings)