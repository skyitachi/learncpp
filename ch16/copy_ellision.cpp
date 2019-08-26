//
// Created by skyitachi on 2019-08-26.
//
#include <iostream>
#include <functional>
#include "../String/MyString.h"

MyString f() {
  return MyString();
}

MyString f2() {
  return MyString("a");
}

// std::function move

typedef std::function<void ()> MessageCallback;
MessageCallback gCallback_;

// 能够接收引用，而不能接收左值?
void setCallback(MessageCallback&& cb) {
  gCallback_ = cb;
}

void cf() {}

void fr(int && f) {

}

template <typename T>
void tf(T&& a) {

}

// 不可避免一次copy和move
void tOri(MyString s) {
  MyString s2 = std::move(s);
}

int main() {
  // in rvalue return, constructor called once
//  MyString s1 = f();
  
  // 使用pure rvalue初始化对象的过程中
  MyString x = MyString(MyString(MyString("a")));
  
  tOri(x);
  std::cout << "after raw value pass: " << x.size() << std::endl;
  
  MyString &x2 = x;
  
  tOri(x2);
  
  std::cout << "after reference value pass: " << x.size() << std::endl;
  
//  MessageCallback cb = cf;
//
//  MessageCallback& cbr = cb;
  // cf 左值
//  setCallback(cf);
//  setCallback(cbr);
//  MyString s2 = f2();
//  fr(b);
//  tf(1);
}
