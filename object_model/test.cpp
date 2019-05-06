//
// Created by skyitachi on 2019-05-06.
//
#include <iostream>

class Base {
public:
  void foo() {}
  virtual void base() {}
private:
  int data;
};

class Derived: public Base {
public:
  void bar() {}
private:
  int derived_data;
  int derived_data2;
};

int main() {
  char c = 'a';
  char* ptr = &c;
  std::cout << "Point Size: " << sizeof(ptr) << std::endl;
  std::cout << "Base Class Size: " << sizeof(Base) << std::endl;
  std::cout << "Derived Class Size: " << sizeof(Derived) << std::endl;
}
