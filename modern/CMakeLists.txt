set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -save-temps")
add_executable(test_modern test.cpp)
target_link_libraries(test_modern jemalloc)

add_executable(test_coroutine coroutine.cpp)

add_executable(variants_demo variants_demo.cpp)

add_executable(data_structure data_structure.cpp)

add_executable(json_demo json_demo.cpp)

add_executable(enum_demo enum_demo.cpp)

add_executable(exception_demo exception.cpp)

add_executable(reference_wrapper_demo reference_wrapper_demo.cpp)