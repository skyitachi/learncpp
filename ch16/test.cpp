//
// Created by skyitachi on 2018/9/13.
//

#include <vector>
#include <cstdio>
#include <iostream>

template <typename C>
void print(const C& c) {
  for(typename C::size_type i = 0; i < c.size(); i++) {
    std::cout << c[i] << " ";
  }
  std::cout << std::endl;
}

template <typename C>
void print2(const C& c) {
  for(typename C::const_iterator it = c.begin(); it != c.end(); it++) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
}

int main() {
  std::vector<int> v1 = {1, 2, 3, 4};

  std::vector<double> v2 = {1, 2, 3, 4};
  print(v1);
  print(v2);
  print2(v1);
  print2(v2);
}

