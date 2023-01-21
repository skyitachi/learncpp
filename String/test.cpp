//
// Created by skyitachi on 2019-06-10.
//

#include "MyString.h"
#include <iostream>
#include <vector>
#include <map>
#include <string_view>
#include <regex>

int main() {
  // const char * constructor
  MyString a = "11";
  // 为什么这个也是copy constructor, b没有被初始化过
  MyString b = a;
  // copy constructor
  MyString c(a);
  
  MyString d;
  // 触发赋值构造
  d = b;
  
  // move constructor
  MyString e(std::move(MyString("move")));
  
  // container
  std::vector<MyString> vec;
  vec.push_back(a);
  std::cout << vec[0].size() << std::endl;
  std::map<MyString, int> m;
  m[b] = 1;
  
  std::string_view s = "hello";
  
  using std::string_literals::operator""s;
  // dangling
  std::string_view bad("hello"s); // bad
  
  std::string_view good("hello");
  
  std::string text = "Quick brown fox.";
  std::regex ws_re("\\s+");
  std::vector<std::string> v(std::sregex_token_iterator(text.begin(), text.end(), ws_re, -1), std::sregex_token_iterator());

  for(auto&& s: v)
    std::cout<<s<<"\n";
}
