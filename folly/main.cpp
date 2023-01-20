//
// Created by skyitachi on 23-1-19.
//
#include <folly/FBString.h>
#include <iostream>
#include <string>

using namespace folly;

int main() {
  {
    auto s1 = folly::fbstring("abc");
    fbstring s2("abcd");
    std::cout << s1 << ", " << s2 << std::endl;
  }

  // verify cow
  {
    folly::fbstring bs1;
    bs1.resize(1000);
    printf("big fb_string s1 data pointer %p\n", bs1.c_str());

    auto bs2 = bs1;
    printf("after copy constructor fb_string bs2 data pointer %p\n", bs2.c_str());
  }
  {
    std::string s1;
    s1.resize(1000);

    printf("big std string s1 data pointer %p\n", s1.c_str());

    auto bs2 = s1;
    printf("after copy constructor bs2 data pointer %p\n", bs2.c_str());
  }
}