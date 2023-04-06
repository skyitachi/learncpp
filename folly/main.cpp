//
// Created by skyitachi on 23-1-19.
//
#include <iostream>
#include <string>
#include <cstring>
#include <folly/FBString.h>
#include <folly/ConcurrentSkipList.h>
#include <string_view>

using namespace folly;

bool myComparator(std::string_view a, std::string_view b) {
  return a < b;
}

struct myComparatorCls {
  bool operator()(const std::string_view& a, const std::string_view& b) {
    return a.length() < b.length();
  }
};

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

  {
    auto accessor = folly::ConcurrentSkipList<int>::create(2);
    std::cout << "insert concurrent skip list success" << std::endl;
    bool success = accessor.add(1);
    if (success) {
      std::cout << "insert first success" << std::endl;
    } else {
      std::cout << "insert failed" << std::endl;
    }
    success = accessor.add(2);
    if (success) {
      std::cout << "insert second success" << std::endl;
    } else {
      std::cout << "insert second failed" << std::endl;
    }

    auto first = accessor.first();
    if (first) {
      std::cout << "first: " << *first << std::endl;
    } else {
      std::cout << "cannot found first" << std::endl;
    }
  }

  {
    auto accessor = folly::ConcurrentSkipList<std::string>::create(2);
    std::string s1 = "hello world";
    accessor.add(s1);
    std::string s2 = "hello world2";
    accessor.add(s2);
    auto first = accessor.first();
    if (first) {
      std::cout << "std::string skiplist first: " << *first << std::endl;
    } else {
      std::cout << "cannot found first" << std::endl;
    }
  }

  {
    auto accessor = folly::ConcurrentSkipList<std::string_view, myComparatorCls>::create(3);
    std::string_view s1 = "hello world long world";
    std::string_view s2 = "hello world2";
    bool success = accessor.add(s1);
    if (success) {
      std::cout << "insert hello world success" << std::endl;
    } else {
      std::cout << "insert failed" << std::endl;
    }
    auto [iter, succ] = accessor.insert(s2);
    if (succ) {
      std::cout << "insert second success" << std::endl;
    } else {
      std::cout << "insert second failed" << std::endl;
    }

    auto first = accessor.first();
    if (first) {
      std::cout << "first: " << *first << std::endl;
    } else {
      std::cout << "cannot found first" << std::endl;
    }
  }
}