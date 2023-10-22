add_executable(rocksdb_demo rocksdb_demo.cpp)
target_link_libraries(rocksdb_demo rocksdb zstd lz4 snappy z)