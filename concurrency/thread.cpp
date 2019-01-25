//
// Created by skyitachi on 2018/12/14.
//

#include <iostream>
#include <stdio.h>
#include <chrono>
#include <thread>
#include "ThreadSafeQueue.h"
#include "ThreadSafeStack.h"

void f1() {
  std::cout << "in the " << std::this_thread::get_id() << " thread\n";
}

void f(int i, std::string const &s) {
  std::cout << i << " " << s << std::endl;
}

// 参数传递
void oops(int some_param) {
  char buffer[1024];
  sprintf(buffer, "%i", some_param);
  // undefined behaviour
//  std::thread t(f, 3, buffer);
  std::thread t(f, 3, std::string(buffer));
  t.detach();
}

// 传引用给 thread
void accept_reference(int &a) {
  a += 1;
}

void pass_reference() {
  int a = 1;
  std::thread t(accept_reference, std::ref(a));
  t.join();
  if (a != 2) {
    std::cout << "pass_reference failed " << std::endl;
  }
}

// 传递成员函数
class X {
public:
  void do_something() {
    std::cout << "called class instance method\n";
  }
};

void pass_class_instance_method() {
  X my_x;
  std::thread t(&X::do_something, &my_x);
  t.join();
}

// 如何返回 thread, thread是可以 move 的直接 return 一般没啥问题
std::thread test_return_thread() {
  return std::thread(f1);
}

template <typename T>
class TClass{
public:
  TClass(): value_(1) {}
  TClass(T v): value_(v) {}
  T value() { return value_; }
private:
  T value_;
};

void test_thread_safe_queue() {
//  TClass<int> a;
//  std::cout << a.value() << std::endl;
  ThreadSafeQueue<int> tsq;
  std::thread t1([&tsq](){
    using namespace std::chrono_literals;
    for(int i = 0; i < 5; i++) {
      std::this_thread::sleep_for(1s);
      tsq.push(i);
    }
  });
  std::thread t2([&tsq](){
    for(int i = 0; i < 5; i++) {
      std::shared_ptr<int> v = tsq.wait_and_pop();
      std::cout << "value is " << *v << std::endl;
    }
  });
  t1.join();
  t2.join();
  if (tsq.empty()) {
    std::cout << "test_thread_safe_queue right\n";
  } else {
    std::cout << "test_thread_safe_queue wrong\n";
  }
}

void test_thread_safe_stack() {
  ThreadSafeStack<int> threadSafeStack;
  std::thread t1([&threadSafeStack]() {
    using namespace std::chrono_literals;
    for(int i = 0; i < 5; i++) {
      threadSafeStack.push(i);
    }
  });
  std::thread t2([&threadSafeStack]() {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);
    for(int i = 0; i < 5; i++) {
      std::shared_ptr<int> vptr = threadSafeStack.pop();
      std::cout << "stack value is : " << *vptr << std::endl;
    }
  });
  t1.join();
  t2.join();
  try {
    threadSafeStack.pop();
  } catch (std::exception &e) {
    std::cout << "catch exception: " << e.what() << std::endl;
  }
  std::cout << "test_thread_safe_stack right\n";
}

int main() {
  std::thread t1(f1);
  std::thread t2 = std::move(t1);
  oops(1);
  t2.join();
  pass_reference();
  pass_class_instance_method();
  std::thread t3 = test_return_thread();
  t3.join();
  test_thread_safe_queue();
  test_thread_safe_stack();
  return 0;
}

