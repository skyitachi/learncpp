cmake_minimum_required(VERSION 3.10)
project(learncpp)

include_directories(/usr/local/include "${CMAKE_SOURCE_DIR}")
LINK_DIRECTORIES(/usr/local/lib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -g")

add_executable(learncpp main.cpp)

add_subdirectory(thirdparty/abseil-cpp)


add_subdirectory(smart_pointers)
add_subdirectory(ch13)

add_subdirectory(ch16)
add_subdirectory(concurrency)
add_subdirectory(system)
add_subdirectory(concurrent_queue)

add_subdirectory(ch9)
add_subdirectory(ch10)
add_subdirectory(ch15)
add_subdirectory(object_model)

add_subdirectory(uv_wrapper)
add_subdirectory(String)

add_subdirectory(zookeeper)
add_subdirectory(leveldb)

add_subdirectory(titan_db)

add_subdirectory(modern)

add_subdirectory(usb)

add_subdirectory(arrow)

add_subdirectory(faiss_demo)


add_subdirectory(openssldemo)

add_subdirectory(kdtreedemo)

add_subdirectory(simd)

add_subdirectory(duckdb)
add_subdirectory(folly)
add_subdirectory(compression_demo)

add_subdirectory(hdfs)
add_subdirectory(abseil_tips_demo)
add_subdirectory(hashmap_demo)

add_subdirectory(module)

add_subdirectory(kv_engine)
add_subdirectory(database_indexing)