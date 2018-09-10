//
// Created by skyitachi on 2018/8/30.
//

#ifndef LEARNCPP_FOO_H
#define LEARNCPP_FOO_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <initializer_list>

class Foo {
public:
  Foo(std::initializer_list<int> il): data(il) {}
  Foo sorted() &&;
  Foo sorted() const &;
private:
  std::vector<int> data;
};


#endif //LEARNCPP_FOO_H
