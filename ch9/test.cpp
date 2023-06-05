//
// Created by skyitachi on 2019-04-04.
//

#include <vector>
#include <iostream>

using namespace std;

// element access
void test_access_container() {
  vector<int> v;
  // error
//  cout << "undefined behavior: " << v.front() << endl;
  // error
//  cout << "undefined behavior: " << v.back() << endl;
//    cout << "v[0]: " << v[0] << endl;
//  v[0];
  try {
    cout << v.at(0);

  } catch (std::exception e) {
    cout << e.what();
  }
}

void test_delete_container() {
  vector<int> v = {1, 2, 3};

  v.erase(v.begin(), v.begin());
  cout << "v.size = " << v.size() << endl;

  // 不会报错，静默失败
  v.erase(v.end(), v.end());

  // it 会不合法
  auto it = v.erase(v.begin(), v.end());
  cout << "v.size = " << v.size() << endl;

}

void test_resize() {
  vector<int> v(2);
  cout << "original: size = " << v.size() << " capacity = " << v.capacity() << endl;
  v.resize(10);
  cout << "resize(10): size = " << v.size() << " capacity = " << v.capacity() << endl;
  v.resize(100);
  cout << "resize(100): size = " << v.size() << " capacity = " << v.capacity() << endl;
  v.push_back(100);
  cout << "push_back(100): size = " << v.size() << " capacity = " << v.capacity() << endl;

}

void test_move() {
  std::vector<int> vec1 = {1, 2, 3};

  std::vector<int> v2 = std::move(vec1);
  std::cout << "v2.size() " << v2.size() << ", v1.size() " << vec1.size() << std::endl;
  vec1.push_back(1);

  std::cout << "v1.size() " << vec1.size() << std::endl;
}

//template<typename T>
//T::size_type count(T::iterator begin, ) {
//
//}

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
  test_access_container();
  test_delete_container();
  test_resize();
  test_move();
  return 0;
}