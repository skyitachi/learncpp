//
// Created by skyitachi on 2018/8/19.
//

#include <stdio.h>
#include <iostream>

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

int main () {
  SmartPointer<int> ptr(new int());
  *ptr = 20;
  std::cout << *ptr << std::endl;
  return 0;
}