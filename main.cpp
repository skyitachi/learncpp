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

int main() {
  const Test* t = new Test(2);
  std::cout <<  t->v << std::endl;
  std::cout << t->getValue() << std::endl;
  
  Test* t1 = new Test(1);
  std::cout << t1->getValue() << std::endl;
  
  const Test t2(2);
//  t->v = 10;
//  t = new Test(3);
  
  std::cout << "Hello, World!" << std::endl;
  return 0;
}