
add_executable(lock_guard lock_guard.cpp)
add_executable(promise promise.cpp)
add_executable(thread thread.cpp ThreadSafeStack.cpp ThreadSafeStack.h parallel_quick_sort.h ThreadSafeQueue.cpp thread_safe_queue_with_lock.h thread_pool.cpp)
add_executable(mutex mutex.cpp)
add_executable(atomic atomic.cpp)
add_executable(cv cv.cpp)
add_executable(concurrent_hashmap_tests concurrent_hashmap_tests.cpp)

add_executable(folly_demo folly_demo.cpp)
target_link_libraries(concurrent_hashmap_tests gtest)
target_link_libraries(mutex boost_thread-mt boost_system-mt pthread)

target_link_libraries(folly_demo folly glog fmt)

find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    message(STATUS "OpenMP found")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_executable(openmp_demo openmp_demo.cpp)