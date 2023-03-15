//
// Created by skyitachi on 23-3-10.
//

#include <iostream>
#include <immintrin.h>
template <class T>
void print(T a, int size) {
  for(int i = 0; i < size; i++) {
    std::cout << a[i] << " ";
  }
  std::cout << std::endl;
}

int main() {
  __m256 a = _mm256_set_ps(8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0);
  __m256 b = _mm256_set_ps(1, 2, 3, 4, 8, 7 , 5, 6);

  float d[8];
  _mm256_storeu_ps(d, _mm256_add_ps(a, b));

  // ? why d[0] == 2.0
  print(d, 8);

  __m256 m = _mm256_max_ps(a, b);
  _mm256_storeu_ps(d, m);
  print(d, 8);
}
