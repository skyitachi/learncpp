//
// Created by skyitachi on 2019/1/25.
//

#include "ThreadSafeStack.h"

template <typename T>
ThreadSafeStack<T>::ThreadSafeStack(const ThreadSafeStack& other) {
  std::lock_guard<std::mutex> lk(mu_);
  data_ = other.data_;
}

template <typename T>
void ThreadSafeStack<T>::push(T new_value) {
  std::lock_guard<std::mutex> lk(mu_);
  data_.push(new_value);
}

template <typename T>
std::shared_ptr<T> ThreadSafeStack<T>::pop() {
  std::lock_guard<std::mutex> lk(mu_);
  if (data_.empty()) {
    throw empty_stack();
  }
  std::shared_ptr<T> const res(std::make_shared<T>(data_.top()));
  data_.pop();
  return res;
}

template <typename T>
void ThreadSafeStack<T>::pop(T& value) {
  std::lock_guard<std::mutex> lk(mu_);
  if (data_.empty()) {
    throw empty_stack();
  }
  value = data_.top();
  data_.pop();
}

template <typename T>
bool ThreadSafeStack<T>::empty() const {
  std::lock_guard<std::mutex> lk(mu_);
  return data_.empty();
}

// Note: class template instantiation
template class ThreadSafeStack<int>;

