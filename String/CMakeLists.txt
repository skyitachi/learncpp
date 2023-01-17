ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
add_executable(mystring test.cpp MyString.cpp)
target_link_libraries(mystring boost_log-mt pthread)

add_executable(fb_string_demo fb_string.cpp)