//
// Created by skyitachi on 2018/9/13.
//

#include <vector>
#include <cstdio>
#include <iostream>
#include <string>
#include <memory>
using std::string;
struct Foo {int i;};

void foo_deleter(Foo* fp, int unused) {
  std::cout << "foo_deleter called" << std::endl;
  delete fp;
}

template <typename It>
auto fcn3(It beg, It end) -> decltype(*beg + 0){
  return *beg;
}

template <typename C>
void print(const C& c) {
  for(typename C::size_type i = 0; i < c.size(); i++) {
    std::cout << c[i] << " ";
  }
  std::cout << std::endl;
}

template <typename C>
void print2(const C& c) {
  for(typename C::const_iterator it = c.begin(); it != c.end(); it++) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
}


template <typename T> class Stack {
  typedef typename T::NotExist StaticAssert;  // used to test if instantiated
};
void f1(Stack<char>);                    // (a) not instantiate
class Exercise {
  Stack<double> &rsd;                    // (b) not instantiate
//  Stack<int> si;                       // (c) instantiate `Stack<int>`
};

template <typename T> int compare(const T&, const T&);

template <typename T> T calc(T, int);

template <typename T> void ft(T&);
template <typename T> void fr(T&&);

void test_right_reference(std::string&& t) {
  std::string s1 = "2";
  std::string& s2 = t;
}

void test_passing_value(std::string& s1) {
  std::cout << "in the left reference" << std::endl;
}

void test_passing_value(const std::string& s1) {
  std::cout << "in the const left reference" << std::endl;
}

void test_passing_value(std::string&& s1) {
  std::cout << "in the right reference" << std::endl;
}

template<typename TR, typename TA>
TR unary_execute(TA input, TR(*udf_func)(TA)) {
  return udf_func(input);
}

template<typename TR, typename... Args>
void callFn(const std::string& name, TR (*udf_func)(Args...)) {
  const std::size_t args_sz = sizeof...(Args);
  std::cout << "args sz: " << args_sz << std::endl;
  // how to call udf_func
  if (args_sz == 1) {
    unary_execute<TR, Args...>((int)1000, udf_func);
  }
}

void fn_unary(int a) {

  std::cout << "custom unary function, arg: " << a << std::endl;
}

void fn(int a, int b) {
  std::cout << "custom udf function" << std::endl;
}

int main() {
//  std::vector<int> v1 = {1, 2, 3, 4};
//
//  std::vector<double> v2 = {1, 2, 3, 4};
//  print(v1);
//  print(v2);
//  print2(v1);
//  print2(v2);
//  Stack<int> *p;
  
//  f1(5);
//  ft(v1);
  
  // 左值右值都可以？
//  fr(v1);
//  fr(5);
  
//  std::cout << sizeof(Stack<string>) << std::endl; // initialize template
  
//  std::shared_ptr<Foo> r(new Foo, foo_deleter);
//  auto dptr = std::get_deleter<void (Foo*, int)>(r);
  // cannot call compare
//  compare("hi", "world");
//  compare("abc", "def");
//  compare<std::string>("hi", "world");
//  double d;
//  float f;
//  char c;
//  calc(c, 'c');
//  fcn3(std::vector<int>().begin(), std::vector<int>().end());
//  fcn3(std::vector<string>().begin(), std::vector<string>().end());

//  std::vector<std::string &> vs;
  std::string s1 = "hello";
  std::string &sr = s1;
  std::string s0 = "h";
  const std::string &csr = s1;
  test_passing_value(s0);
  test_passing_value(std::move(s0));
  test_passing_value(sr);
  test_passing_value(std::move(csr));


  callFn("my_udf", &fn_unary);
}

//void f1(Stack<char> a) { // initialize template
//
//}

