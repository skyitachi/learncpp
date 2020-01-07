set(CMAKE_CXX_STANDARD 11)

include_directories(BEFORE /Users/skyitachi/lab/pingcap_rocksdb/include)
include_directories(BEFORE /Users/skyitachi/lab/titan/include)

link_directories(/Users/skyitachi/lab/titan/cmake-build-debug)
link_directories(/Users/skyitachi/lab/titan/cmake-build-debug/rocksdb)

add_executable(titan_test titandb_test.cpp)
target_link_libraries(titan_test rocksdb titan snappy)
