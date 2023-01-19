//
// Created by skyitachi on 23-1-19.
//
#include <folly/FBString.h>
#include <iostream>

using namespace folly;

int main() {
  auto s1 = folly::fbstring("abc");
  fbstring s2("abcd");
  std::cout << s1 << ", " << s2 << std::endl;
}