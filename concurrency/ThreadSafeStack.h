//
// Created by skyitachi on 2019/1/25.
//

#ifndef LEARNCPP_THREADSAFESTACK_H
#define LEARNCPP_THREADSAFESTACK_H

#include <memory>
#include <exception>
#include <mutex>
#include <stack>

struct empty_stack: std::exception {
  // Note: throw
  virtual char const * what() const noexcept { return "empty stack"; }
};

template <typename T>
class ThreadSafeStack {
public:
  ThreadSafeStack() = default;
  ThreadSafeStack(const ThreadSafeStack&);

  ThreadSafeStack&operator=(const ThreadSafeStack&) = delete;
  void push(T new_value);
  std::shared_ptr<T> pop();
  void pop(T& value);

  bool empty() const;
private:
  mutable std::mutex mu_;
  std::stack<T> data_;
};


#endif //LEARNCPP_THREADSAFESTACK_H
