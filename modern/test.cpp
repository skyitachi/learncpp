//
// Created by skyitachi on 2020/1/8.
//
#include <optional>
#include <string>
#include <iostream>
#include <map>
// std::optional has a some memory overhead
class UserName {
public:
  explicit UserName(const std::string &str) : mName(str) {
      std::cout << "UserName constructor( " << str << ")\n";
  }

  ~UserName() {
      std::cout << "UserName destructor (" << mName << ")\n";
  }

private:
  std::string mName;
};

struct Point {
  int x, y;
};

int main() {
    std::optional<std::string> s = std::nullopt;
    if (s) {
        std::cout << "wrong answer\n";
    } else {
        std::cout << "right \n";
    }

    std::optional<int> oint = 10;
    std::cout << "oint: " << *oint << std::endl;
    std::cout << "oint: " << oint.value() << std::endl;

    std::optional<double> oduble;
    std::cout << "odouble " << oduble.value_or(10.0) << std::endl;

    std::optional<UserName> oEmpty;
    oEmpty.emplace("ysp");

    oEmpty.emplace("skyitachi");

    // NOTE: it will not call destructor
    oEmpty = UserName("abc");
    oEmpty.reset();
    std::cout << "end of program\n";
    // great
    if (auto a = true; a) {
      std::cout << "go like if condition\n";
    }
    // structured binding
    Point p = {1, 2};
    auto [x, y] = p;
    std::cout << "x is " << x << " y is " << y << std::endl;

  {
    int arr[3] = {1, 2, 3};
    auto [x, y, z] = arr;
    std::cout << x << " " << y << " " << z << std::endl;
  }
  {
    // great
    std::map<std::string, std::string> m;
    m.insert({"a", "1"});
    m.insert({"b", "2"});
    for (auto &[k, v]: m) {
      printf("key=%s, value=%s\n", k.c_str(), v.c_str());
    }
  }
}