//
// Created by Shiping Yao on 2023/3/5.
//
#include <iostream>
#include <string_view>
#include <absl/strings/str_cat.h>
#include <absl/strings/str_join.h>
#include <absl/strings/str_replace.h>
#include <absl/strings/substitute.h>

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
  {
    std::cout << absl::Substitute("Hello substitute $0 ($1)", "abseil", 42) << std::endl;
  }

  {
    std::vector<std::string> v = {"a", "b", "c"};
    std::string s = absl::StrJoin(v, "-");
    std::cout << "first join: " << s << std::endl;

    std::vector<std::string_view> v2 = {"a", "b", "c"};
    std::string s2 = absl::StrJoin(v, "-");

    std::cout << "join string_view: " << s2 << std::endl;

    std::vector<int> iv = {1, 2, 3};
    std::string s3 = absl::StrJoin(iv, "-");
    std::cout << "join int vector: " << s3 << std::endl;

    const int a[] = {1, 2, 3};
    std::string s4 = absl::StrJoin(a, "-");
    std::cout << "join int array: " << s4 << std::endl;

    std::map<std::string, int> m = {{"a", 1}, {"b", 2}, {"c", 3}};
    std::string s5 = absl::StrJoin(m, ";", absl::PairFormatter("="));
    std::cout << "join map: " << s5 << std::endl;
  }
}
