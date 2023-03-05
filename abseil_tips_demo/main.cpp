//
// Created by Shiping Yao on 2023/3/5.
//
#include <iostream>
#include <string_view>
#include <absl/strings/str_cat.h>
#include <absl/strings/str_join.h>

void TakesStringView(std::string_view s) {

}

void AlreadyHasString(const std::string& s) {
  TakesStringView(s);
}

void AlreadyHasCharStar(const char* s) {
  TakesStringView(s);
}

int main() {
  std::string_view sv{"hello world"};
  std::cout << sv << std::endl;

  {
    auto ret = absl::StrCat("hello world", " hello abseil");
    std::cout << ret << std::endl;
  }
}
