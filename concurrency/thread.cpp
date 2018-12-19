//
// Created by skyitachi on 2018/12/14.
//

#include <thread>
#include <iostream>
#include <stdio.h>

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

int main() {
  std::thread t1(f1);
  std::thread t2 = std::move(t1);
  oops(1);
  t2.join();
  pass_reference();
  pass_class_instance_method();
  std::thread t3 = test_return_thread();
  t3.join();
  return 0;
}

