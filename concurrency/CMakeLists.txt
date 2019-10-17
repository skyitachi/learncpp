
add_executable(lock_guard lock_guard.cpp)
add_executable(promise promise.cpp)
add_executable(thread thread.cpp ThreadSafeStack.cpp ThreadSafeStack.h parallel_quick_sort.h ThreadSafeQueue.cpp)
add_executable(mutex mutex.cpp)
add_executable(atomic atomic.cpp)
target_link_libraries(mutex boost_thread-mt boost_system-mt pthread)
