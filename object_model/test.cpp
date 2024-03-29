//
// Created by skyitachi on 2019-05-06.
//
#include <iostream>
#include <cstddef>

class Base {
public:
  void foo(double y, double x) {
    std::cout << "base foo double x" << x <<  ", y: " << y << std::endl;
  }
  virtual void base() {}

  virtual void foo(int x) = 0 ;

private:
  int data;
};

class Derived: public Base {
public:
  void bar() {}
  void foo(int x) override {
    std::cout << "derived foo int x " << x << std::endl;
  }
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
  static const int chunkSize = 250;
public:
  int x;
  int y;
};

void testDataMemberAddr() {
  Point3d point3d;
  point3d.y = 0;
  std::cout << &point3d.y << std::endl;
  std::cout << &point3d << std::endl;
  std::cout << offsetof(Point3d, x) << std::endl;
  std::cout << offsetof(Point3d, y) << std::endl;
  printf("&Point3d::x = %p\n", &Point3d::x);
  printf("&Point3d::y = %p\n", &Point3d::y);
  // how get data member offset of class
//  if (&point3d.y == &point3d + (&(Point3d::y) - 1)) {
//    std::cout << "data member address correctly" << std::endl;
//  }
}

class Point {
public:
  virtual ~Point();
  float x();
  float y();
  virtual float z();
};

float Point::x() { return 1.0; }
float Point::y() { return 1.0; }

void testFunctionMember() {
//  std::cout << &Point::~Point << std::endl;
// C++对象模型的中的做法
  std::cout << "Point::y() addr: " << &Point::y << std::endl;
  std::cout << "Point::z() addr: " << &Point::z << std::endl;
}

void testBaseAndDerive() {
  Derived d;
  d.foo(1.0);
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
  testFunctionMember();
  testBaseAndDerive();
}
