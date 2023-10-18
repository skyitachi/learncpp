//
// Created by skyitachi on 23-10-17.
//

#include <algorithm>
#include <vector>
#include <iostream>

int main() {
  std::vector<int> v = {1, 2, 3, 4, 5};

  // Make the vector into a heap. not sort
  std::make_heap(v.begin(), v.end());

  // Print the heap.
  for (int i = 0; i < v.size(); i++) {
    std::cout << v[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}
