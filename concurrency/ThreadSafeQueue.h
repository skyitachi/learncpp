//
// Created by skyitachi on 2019/1/24.
//

#ifndef LEARNCPP_THREADSAFEQUEUE_H
#define LEARNCPP_THREADSAFEQUEUE_H

#include <memory>
#include <mutex>
#include <condition_variable>
#include <queue>

template<typename T>
class ThreadSafeQueue {
public:
  ThreadSafeQueue() = default;
  ThreadSafeQueue(ThreadSafeQueue const&);
  ThreadSafeQueue&operator= (const ThreadSafeQueue&) = delete;

  void push(T new_value);
  bool try_pop(T& value);
  std::shared_ptr<T> try_pop();

  void wait_and_pop(T& value);
  std::shared_ptr<T> wait_and_pop();

  bool empty() const;

private:
  // Note: mutable
  mutable std::mutex mu_;
  std::queue<T> queue_;
  std::condition_variable cond_;
};

#include "ThreadSafeQueue.tpp"

#endif //LEARNCPP_THREADSAFEQUEUE_H
