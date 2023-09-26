//
// Created by Shiping Yao on 2023/9/26.
//
#include <string>
#include <charconv>
#include <iostream>

void parse_float() {
  std::string s = "123456789111";
  uint64_t val;
  auto [ptr, ec] =
      std::from_chars(s.data(), s.data() + s.size(), val);

  std::cout << "parsed val: " << val << std::endl;
}

int main() {
  parse_float();
}