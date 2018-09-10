//
// Created by skyitachi on 2018/8/30.
//

#include "Foo.h"

Foo Foo::sorted() && {
  std::cout << "in the right value sort" << std::endl;
  std::sort(data.begin(), data.end());
  return *this;
}

//Foo Foo::sorted() const & {
//  std::cout << "in the left value sort" << std::endl;
//  Foo ret(*this);
//  std::sort(ret.data.begin(), ret.data.end());
//  return ret;
//}

Foo Foo::sorted() const &{
  return Foo(*this).sorted();
}

// infinite loop
//Foo Foo::sorted() const &{
//  Foo ret(*this);
//  return ret.sorted();
//}
