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

class X {};
class Y: public virtual X {};
class Z: public virtual X {};
class A: public Y, public Z {};

class Point3d {
private:
  float x;
  static const int chunkSize = 250;
public:
  int y;
};

void testDataMemberAddr() {
  Point3d point3d;
  point3d.y = 0;
  // how get data member offset of class
//  if (&point3d.y == &point3d + (&(Point3d::y) - 1)) {
//    std::cout << "data member address correctly" << std::endl;
//  }
}

int main() {
  char c = 'a';
  char* ptr = &c;
  std::cout << "Point Size: " << sizeof(ptr) << std::endl;
  std::cout << "Base Class Size: " << sizeof(Base) << std::endl;
  std::cout << "Derived Class Size: " << sizeof(Derived) << std::endl;
  
  
  std::cout << "sizeof(X) = " << sizeof(X) << std::endl;
  std::cout << "sizeof(Y) = " << sizeof(Y) << std::endl;
  std::cout << "sizeof(Z) = " << sizeof(Z) << std::endl;
  std::cout << "sizeof(A) = " << sizeof(A) << std::endl;

  std::cout << "sizeof(Point3d) = " << sizeof(Point3d) << std::endl;

  testDataMemberAddr();
}
