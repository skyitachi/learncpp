link_directories(/Users/skyitachi/lab/leveldb/cmake-build-debug)
#link_directories(/Users/skyitachi/lab/rocksdb/cmake-build-debug)
add_executable(test_level test_level.cpp)
target_link_libraries(test_level leveldb snappy)

add_executable(parse ldb_parser.cpp)
target_link_libraries(parse ByteBufferCpp leveldb snappy)

add_executable(rocks_example rocks_example.cpp)
target_link_libraries(rocks_example rocksdb)

add_executable(rocks_transaction rocks_transaction.cpp)
target_link_libraries(rocks_transaction rocksdb)

add_executable(comparator comparator.cpp)
target_link_libraries(comparator rocksdb)

add_executable(column_families column_families.cpp)
target_link_libraries(column_families rocksdb)

add_executable(inlineskiplist_debug inlineskiplist_debug.cpp)
target_link_libraries(inlineskiplist_debug rocksdb)

add_executable(rocks_opt_transaction rocks_opt_transaction.cpp)
target_link_libraries(rocks_opt_transaction rocksdb)

add_executable(rocks_merge_operator rocks_merge_operator.cpp)
target_link_libraries(rocks_merge_operator rocksdb)

