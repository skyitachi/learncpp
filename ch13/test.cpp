//
// Created by skyitachi on 2018/8/24.
//

#include <stdio.h>
#include <string>

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
}
