add_executable(fbstring_demo main.cpp)
target_link_libraries(fbstring_demo folly fmt glog sqlparser)


add_executable(executor_demo executor_demo.cpp)
target_link_libraries(executor_demo folly fmt glog)