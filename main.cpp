#include <iostream>

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
  return 0;
}
