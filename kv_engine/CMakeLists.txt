if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    add_compile_options(-fno-rtti)
endif ()

add_executable(rocksdb_demo rocksdb_demo.cpp)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    target_link_libraries(rocksdb_demo rocksdb zstd lz4 snappy z uring bz2)
else ()
    target_link_libraries(rocksdb_demo rocksdb zstd lz4 snappy z)
endif ()

