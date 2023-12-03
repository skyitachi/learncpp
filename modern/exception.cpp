//
// Created by Shiping Yao on 2023/12/3.
//

#include <iostream>

#include <exception>

int main() {
  int age;

  try {
    std::cout << "Enter your age: ";
    std::cin >> age;

    if (age < 18) {
//      throw std::exception("You must be 18 or older to enter");
      throw std::invalid_argument("You must be 18 or older to enter");
    }
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    return 1;
  }

  std::cout << "Welcome to the party!" << std::endl;

  return 0;
}
