add_executable(test_level test_level.cpp)
target_link_libraries(test_level leveldb snappy)

add_executable(parse ldb_parser.cpp)