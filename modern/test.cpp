//
// Created by skyitachi on 2020/1/8.
//
#include <optional>
#include <string>
#include <iostream>
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
}