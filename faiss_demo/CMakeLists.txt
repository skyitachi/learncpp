set(CMAKE_CXX_STANDARD 17)

add_executable(faiss_demo main.cpp)
target_link_libraries(faiss_demo faiss)