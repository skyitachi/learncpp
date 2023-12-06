//
// Created by Shiping Yao on 2023/12/6.
//

#include <iostream>

int main() {
  int a = 10;
  std::reference_wrapper<int> ref_int(a);

  int b = 5;

  // change under layer value
  ref_int.get() = 2;

  ref_int = b;

  std::cout << "after assign: a = " << a << std::endl;

  ref_int.get() = 2;

  std::cout << "after assign: b = " << b << std::endl;

}
