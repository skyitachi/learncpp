set(CMAKE_CXX_FLAGS "-std=c++11")
add_executable(ch16 test.cpp)

ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
add_executable(copy_ellision copy_ellision.cpp ../String/MyString.cpp)

target_link_libraries(copy_ellision boost_log-mt pthread)
