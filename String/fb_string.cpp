//
// Created by skyitachi on 23-1-17.
//

#include <string>
#include <iostream>
#include <stdio.h>

// SSO small string optimization
class SSOString {

  char *start;
  size_t size;
  static const int kLocalSize = 15;
  union {
    char buffer[kLocalSize + 1];
    size_t capacity;
  };
};

int main() {
  std::string s("123");

  const char* p = s.data();
  printf("origin data ptr: %p\n", p);

  {
    std::string s2(s);
    (void) s[0];
  }
  // 并没有发生cow, 现在版本的c++没有这个问题了
  printf("after cow data ptr: %p\n", s.data());
  std::cout << *p << std::endl;
}
