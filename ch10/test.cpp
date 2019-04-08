//
// Created by skyitachi on 2019-04-08.
//

#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>

using namespace std;

int main() {
  vector<double> v = {1.1, 2.2};
  // 0 as int, it will lose decimals
//  cout << accumulate(v.cbegin(), v.cend(), 0) << endl;
  cout << accumulate(v.cbegin(), v.cend(), 0.0) << endl;
}
