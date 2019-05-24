//
// Created by skyitachi on 2018/8/19.
//

#include <stdio.h>
#include <iostream>
#include <memory>

#include "StrBlob.h"
#include "StrBlobPtr.h"

template <class T>
class SmartPointer {
  T *ptr;
public:
  explicit SmartPointer(T *p = NULL) {
    ptr = p;
  }
  ~SmartPointer() {
    delete (ptr);
  }
  T &operator* () { return *ptr; }

  T *operator->() { return ptr; }
};
typedef void (*Callback)();


class Foo {
public:
  ~Foo() {
    std::cout << "foo destructor called\n";
  }
  void sayHello () { std::cout << "hello \n"; }
};

void test_in_callback(Callback cb) {
  cb();
}

int main () {
  SmartPointer<int> ptr(new int());
  *ptr = 20;
  std::cout << *ptr << std::endl;
  StrBlob blob{"a", "b"};
  std::cout << blob.size() << std::endl;
  blob.push_back("hello");
  std::string s1 = "hello";
  blob.push_back(s1);
  test_in_callback([]() {
    // callback里分配的资源，如果不借助外力的话一定会被回收的
    auto uniqueFoo = std::make_unique<Foo>();
    uniqueFoo->sayHello();
  });
  return 0;
}