//
// Created by Shiping Yao on 2023/12/6.
//

#include <iostream>

int ga = 10;
void test_pass_ref(std::reference_wrapper<int> &v) {
//  int a = 10;
  v = ga;
}

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

  test_pass_ref(ref_int);

  std::cout << "after test_pass_ref fun: " << a << std::endl;

  std::cout << "after test_pass_ref fun: " << b << std::endl;

  std::cout << "after test_pass_ref origin ref_int value: " << ref_int << std::endl;

  ref_int.get() = 100;

  std::cout << "ga now is: " << ga << std::endl;

}
