ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
add_executable(mystring test.cpp MyString.cpp)
target_link_libraries(mystring boost_log-mt pthread)