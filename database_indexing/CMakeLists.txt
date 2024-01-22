add_executable(fstdemo fst_demo.cpp
        art_demo.cpp)

target_link_libraries(fstdemo fst)

add_executable(art_demo art_demo.cpp
        thmap/thmap.h)

target_link_libraries(art_demo part fmt)

add_library(thmap thmap/thmap.c thmap/t_thmap.cpp thmap/murmurhash.c)

add_executable(t_stress thmap/t_stress.cpp)

target_link_libraries(t_stress thmap)

add_executable(thmap_demo thmap_demo.cpp)

target_link_libraries(thmap_demo thmap)