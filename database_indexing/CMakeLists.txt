add_executable(fstdemo fst_demo.cpp
        art_demo.cpp)

target_link_libraries(fstdemo fst)

add_executable(art_demo art_demo.cpp)

target_link_libraries(art_demo part fmt)