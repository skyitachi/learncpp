add_executable(arrow_demo arrow_demo.cpp)

target_link_libraries(arrow_demo arrow)


add_executable(plasma_demo plasma_demo.cpp)
target_link_libraries(plasma_demo plasma arrow)


add_executable(fixed_schema_demo arrow_fixed_schema_demo.cpp)
target_link_libraries(fixed_schema_demo arrow)