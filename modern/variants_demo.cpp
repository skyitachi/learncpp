//
// Created by Shiping Yao on 2023/10/17.
//
#include <iostream>

#include <variant>

int main() {
  std::variant<int, std::string> v;

  v = 100;

  std::cout << "int value: " << std::get<int>(v) << std::endl;

  v = "hello world";

  std::cout << "string value: " << std::get<std::string>(v) << std::endl;

  std::visit([](auto &&arg) {
    std::cout << "std::visit: " << arg << "\n";
  }, v);

  v = 10000;

  std::string s = std::visit([](auto &&arg) -> std::string {
    if constexpr (std::is_same_v<std::string, std::decay_t<decltype(arg)>>) {
      return arg;
    } else {
      return std::to_string(arg);
    }
  }, v);

  std::cout << "transformed string value: " << s << std::endl;
}
