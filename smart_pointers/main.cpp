//
// Created by skyitachi on 2018/8/19.
//

#include <stdio.h>
#include <iostream>
#include <memory>
#include <utility>

#include "StrBlob.h"
#include "StrBlobPtr.h"

int gCounter = 0;
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

struct Unique {
  Unique() {
    id_ = gCounter++;
  }
  ~Unique() {
    std::cout << "Unique << "  << id_ << " destroyed\n";
  }
  
  inline int Id() {
    return id_;
  }

private:
  int id_;
};

class Foo {
public:
  Foo(): uPtr(std::make_unique<Unique>()) {
  
  }
  ~Foo() {
    std::cout << "foo destructor called\n";
  }
  void sayHello () { std::cout << "hello \n"; }

private:
  std::unique_ptr<Unique> uPtr;
};


void test_in_callback(Callback cb) {
  cb();
}

void testArgUniquePtr(std::unique_ptr<Foo> fooPtr) {
  fooPtr->sayHello();
}

void testUnqiuePtr() {
  auto foo = std::make_unique<Foo>();
  testArgUniquePtr(std::move(foo));
  std::cout << "foo already destroyed\n";
}

void receiveSharedPtr(std::shared_ptr<Foo> ptr) {
  std::cout << "in the receiveSharedPtr\n";
  ptr->sayHello();
  std::cout << "leave the receiveSharedPtr\n";
}

std::shared_ptr<Foo> testSharedPtrHasUniquePtrDataMember() {
  std::cout << "in the testSharedPtrHasUniquePtrDataMember\n";
  auto fooPtr = std::make_shared<Foo>();
  receiveSharedPtr(fooPtr);
  std::cout << "leave the testSharedPtrHasUniquePtrDataMember\n";
  return fooPtr;
}

class Test {
public:
  Test(): id_(gCounter++) {
    std::cout << "default constructor " << id_ << std::endl;
  }
  Test(Test &&t) {
    id_ = t.id_;
    std::cout << "move constructor" <<  id_ << std::endl;
  }
  
  ~Test() {
    std::cout << "destructor " << id_ << std::endl;
  }
private:
  int id_;
};

Test createTest() {
  return Test();
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
  testUnqiuePtr();
  std::cout << "-------------------------\n";
  auto sharedPtr = testSharedPtrHasUniquePtrDataMember();
  sharedPtr->sayHello();
  Foo* rawFooPtr = new Foo();
  std::cout << "sizeof(Foo*) = " << sizeof(rawFooPtr) << std::endl;
  std::cout << "sizeof(shared_ptr<Foo>) = " << sizeof(sharedPtr) << std::endl;

  std::cout << "-------------------------\n";
  Test t(createTest());
  return 0;
}