//
// Created by skyitachi on 23-1-19.
//
#include <iostream>
#include <string>
#include <folly/FBString.h>

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

    // cow
    auto bs2 = bs1;
    printf("after copy constructor fb_string bs2 data pointer %p\n", bs2.c_str());
    bs1[1] = 'c';

    printf("verify fb_string cow effect data pointer: %p\n", bs1.c_str());
  }
  {
    std::string s1;
    s1.resize(1000);

    printf("big std string s1 data pointer %p\n", s1.c_str());

    auto bs2 = s1;
    printf("after copy constructor std data pointer %p\n", bs2.c_str());
  }

  // verify medium string
  {
    fbstring s1;
    s1.resize(200);

    printf("medium fbstring data pointer: %p\n", s1.c_str());

    fbstring s2 = s1;
    printf("medium fbstring after copy new data pointer: %p\n", s2.c_str());
  }
}