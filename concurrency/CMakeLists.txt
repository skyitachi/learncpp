
add_executable(lock_guard lock_guard.cpp)
add_executable(promise promise.cpp)
add_executable(thread thread.cpp)
add_executable(mutex mutex.cpp)
target_link_libraries(mutex boost_thread-mt boost_system-mt pthread)
