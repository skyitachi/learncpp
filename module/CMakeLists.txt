add_executable(hello_module use.cpp Hello.cppm)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET hello_module PROPERTY CXX_STANDARD 20)
endif()

add_compile_options(-fmodules-ts)