#include <iostream>
#include <map>
#include <string>
#include <cstring>

class Test {
public:
  Test(int v_): v(v_) {};
  int v;
  int getValue() {
    std::cout << "in the getValue" << std::endl;
    return v;
  }
  int getValue() const {
    std::cout << "in the const getValue" << std::endl;
    return v;
  }
};

class A {
public:
  explicit A(int a):a_(a) {}
  // A(int a):a_(a) {}
  int value() {
    return a_;
  }
private:
  int a_;
};

// nocopyable
class NoCopyable {
public:
  NoCopyable(int a): value(a) {};
  int Value() { return value; }
  NoCopyable(const NoCopyable&) = delete;
  NoCopyable &operator=(const NoCopyable&) = delete;

private:
  int value;
};

// Note: std::map is sorted
void test_map() {
  std::map<std::string, int> m;
  m["a"] = 1;
  m["b"] = 2;
  m["a1"] = 3;
  auto it = m.begin();
  for(;it != m.end(); it++) {
    std::cout << it->first << " = " << it->second << std::endl;
  }
  char c1[8];
  char c2[8];
  int64_t i1 = 0;
  int64_t i2 = 10;
  memcpy(c1, &i2, sizeof(int64_t));
  memcpy(c2, &i1, sizeof(int64_t));
  std::string s1(c1, 8);
  std::string s2(c2, 8);
  m.clear();
  m[s1] = 1;
  m[s2] = 2;

  for(auto item: m) {
    std::cout << item.second << std::endl;
  }

  // lower_bound and upper_bound
  char c3[8];
  int64_t i3 = 1;
  memcpy(c3, &i3, sizeof(int64_t));
  std::string s3(c3, 8);
  auto lower = m.lower_bound(s3);
  if (lower != m.end()) {
    std::cout << "lower_bound " << lower->second << std::endl;
  }
  auto upper = m.upper_bound(s3);
  if (upper != m.end()) {
    std::cout << "upper_bound " << upper->second << std::endl;
  }
}

int main() {
  Test* t = new Test(2);
  std::cout <<  t->v << std::endl;
  std::cout << t->getValue() << std::endl;
  
  Test* t1 = new Test(1);
  std::cout << t1->getValue() << std::endl;
  
  const Test t2(2);
//  t->v = 10;
//  t = new Test(3);
  
  std::cout << "Hello, World!" << std::endl;

  // cannot implicit trans
//  A a = {1};
//  std::cout << "explicit constructor" << std::endl;
  NoCopyable nv1(2);
//  NoCopyable nv2(nv1);
//  NoCopyable nv2 = nv1;
  std::map<const std::string, int> memoryIndex;
  std::string s1 = "hello world";
  memoryIndex[s1] = 1;
  std::cout << "memory index: " << memoryIndex[s1] << std::endl;
  if (memoryIndex.find("hello world") != memoryIndex.end()) {
    std::cout << "right " << std::endl;
  }
  char buf[8];
  int64_t i = 0;
  memcpy(buf, &i, sizeof(int64_t));
  std::string s2;
  s2.assign(buf, 8);
  memoryIndex[s2] = 1;
  char buf2[8];
  int64_t j = 0;
  memcpy(buf2, &j, sizeof(int64_t));
  if (memoryIndex.find(std::string(buf2, 8)) != memoryIndex.end()) {
    std::cout << "right " << std::endl;
  }
  test_map();
  return 0;
}
