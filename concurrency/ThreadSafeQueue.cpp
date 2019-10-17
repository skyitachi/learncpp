//
// Created by skyitachi on 2019/1/24.
//

#include "ThreadSafeQueue.h"

template<typename T>
ThreadSafeQueue<T>::ThreadSafeQueue(ThreadSafeQueue const&other) {
  std::lock_guard<std::mutex> lk(other.mu_);
  queue_ = other.queue_;
}

template<typename T>
void ThreadSafeQueue<T>::push(T new_value) {
  std::lock_guard<std::mutex> lk(mu_);
  queue_.push(new_value);
  cond_.notify_one();
}

template<typename T>
bool ThreadSafeQueue<T>::try_pop(T &value) {
  std::lock_guard<std::mutex> lk(mu_);
  if (queue_.empty()) {
    return false;
  }
  value = queue_.front();
  queue_.pop();
}

template<typename T>
std::shared_ptr<T> ThreadSafeQueue<T>::try_pop() {
  std::lock_guard<std::mutex> lk(mu_);
  if (queue_.empty()) {
    return nullptr;
  }
  T& value = queue_.front();
  queue_.pop();
  return std::make_shared<T>(value);
}

template <typename T>
bool ThreadSafeQueue<T>::empty() const {
  std::lock_guard<std::mutex> lk(mu_);
  return queue_.empty();
}

template <typename T>
void ThreadSafeQueue<T>::wait_and_pop(T& value) {
  std::unique_lock<std::mutex> lk(mu_);
  cond_.wait(lk, [this]{ return !queue_.empty(); });
  value = queue_.front();
  queue_.pop();
}

template <typename T>
std::shared_ptr<T> ThreadSafeQueue<T>::wait_and_pop() {
  std::unique_lock<std::mutex> lk(mu_);
  cond_.wait(lk, [this]{ return !queue_.empty(); });
  std::shared_ptr<T> res(std::make_shared<T>(queue_.front()));
  queue_.pop();
  return res;
}

