
add_executable(lock_guard lock_guard.cpp)
add_executable(promise promise.cpp)
add_executable(thread thread.cpp ThreadSafeStack.cpp ThreadSafeStack.h parallel_quick_sort.h ThreadSafeQueue.cpp thread_safe_queue_with_lock.h thread_pool.cpp)
add_executable(mutex mutex.cpp)
add_executable(atomic atomic.cpp)
add_executable(cv cv.cpp)
add_executable(concurrent_hashmap_tests concurrent_hashmap.cpp concurrent_hashmap_tests.cpp)
target_link_libraries(mutex boost_thread-mt boost_system-mt pthread)
