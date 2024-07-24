add_executable(faiss_demo main.cpp)
target_link_libraries(faiss_demo faiss)

message(STATUS ${CMAKE_CXX_COMPILER_ID})

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    SET( CMAKE_CXX_FLAGS  "-Ofast -lrt -std=c++11 -DHAVE_CXX0X -march=native -fpic -w -fopenmp -ftree-vectorize -ftree-vectorizer-verbose=0" )
endif ()

add_executable(hnswlib_demo hnswlib_demo.cpp)

find_package(BLAS REQUIRED)

add_executable(faiss_hnsw_demo faiss_hnsw_demo.cpp)
target_link_libraries(faiss_hnsw_demo faiss ${BLAS_LIBRARIES})