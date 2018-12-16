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

int main() {
  std::thread t1(f1);
  std::thread t2 = std::move(t1);
  oops(1);
  t2.join();
  return 0;
}

