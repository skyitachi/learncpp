//
// Created by skyitachi on 2019-04-04.
//

#include <vector>
#include <iostream>

using namespace std;

int main() {
  vector<int> v(10);
  auto it1 = v.begin() + 2;
  auto it2 = v.begin();
  int c = 0;
  while(it1 != it2) {
    it1++;
    if (c++ > 10) {
      cout << " illegal interval\n";
      break;
    }
  }
  return 0;
}