add_executable(duckdb_demo main.cpp)
link_directories(/home/skyitachi/lab/duckdb/cmake-build-debug/src)
if (APPLE)
    target_link_libraries(duckdb_demo duckdb)
else()
    target_link_libraries(duckdb_demo asan duckdb)
endif()
