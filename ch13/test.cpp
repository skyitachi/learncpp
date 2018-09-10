//
// Created by skyitachi on 2018/8/24.
//

#include <stdio.h>
#include <string>
#include <iostream>
#include "StrVec.h"
#include "Foo.h"

struct NoDtor {
  NoDtor() = default;
  ~NoDtor() = delete;
};

class HasPtr {
public:
  HasPtr(const std::string &s = std::string()): ps(new std::string(s)), i(0) {}
  HasPtr(const HasPtr& rhs):
      ps(new std::string(*rhs.ps)), i(rhs.i) {}
  HasPtr &operator=(const HasPtr&);
  ~HasPtr() {
    printf("in the destructor");
    delete ps;
  }
  void content() {
    printf("content is: %s\n", ps->c_str());
  }
private:
  std::string *ps;
  int i;
};

HasPtr &HasPtr::operator=(const HasPtr & rhs) {
  auto newp = new std::string(*rhs.ps);
  delete ps;
  ps = newp;
  i = rhs.i;
  return *this;
}

int main() {
//  NoDtor nd;
  NoDtor * p = new NoDtor();
//  delete p;
  HasPtr a("hello");
  a = a;
  a.content();
  StrVec sVec;
  sVec.push_back("hello world");
  std::cout << "size is: " << sVec.size() << std::endl;
  sVec.push_back("string 2");
  std::cout << "size is: " << sVec.size() << std::endl;
  sVec.push_back("string 3");
  std::cout << "size is: " << sVec.size() << " cap: is " << sVec.capacity() << std::endl;
  StrVec sv2 = sVec;
  std::cout << "size is: " << sv2.size() << " cap: is " << sv2.capacity() << std::endl;

  StrVec sv3 = std::move(StrVec{"hello", "world"});
  std::cout << "size is: " << sv3.size() << " cap: is " << sv3.capacity() << std::endl;

  sv3 = std::move(StrVec{"1", "2", "3"});
  std::cout << "size is: " << sv3.size() << " cap: is " << sv3.capacity() << std::endl;

  Foo foo{3, 2, 1};
  Foo foo1 = foo.sorted();
}
