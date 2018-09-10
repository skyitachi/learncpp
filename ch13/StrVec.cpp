//
// Created by skyitachi on 2018/8/27.
//

#include <iostream>
#include "StrVec.h"

std::allocator<std::string> StrVec::alloc;

StrVec::~StrVec() {
  free();
}

StrVec::StrVec(std::initializer_list<std::string> il) {
  for(const auto &s : il) {
    push_back(s);
  }
}

StrVec::StrVec(const StrVec &rhs) {
  if (rhs.elements) {
    std::pair<std::string *, std::string *> copy = alloc_n_copy(rhs.elements, rhs.first_free);
    elements = copy.first;
    first_free = copy.second;
    cap = first_free;
  }
}


StrVec& StrVec::operator=(const StrVec &rhs) {
  if (rhs.elements) {
    std::pair<std::string *, std::string *> copy = alloc_n_copy(rhs.elements, rhs.first_free);
    elements = copy.first;
    first_free = copy.second;
    cap = first_free;
  }
  return *this;
}

// Note: 移动构造函数
StrVec::StrVec(StrVec &&rhs) noexcept {
  std::cout << "Debug: " << "in the move constructor" << std::endl;
  if (this != &rhs) {
    // Note: why no free(), 构造函数没必要用free
//    free();
    elements = rhs.elements;
    first_free = rhs.first_free;
    cap = rhs.cap;
    rhs.elements = rhs.first_free = rhs.cap = nullptr;
  }
}

StrVec& StrVec::operator=(StrVec &&rhs) {
  std::cout << "Debug: " << "in the move operator" << std::endl;
  if (this != &rhs) {
    free();
    elements = rhs.elements;
    first_free = rhs.first_free;
    cap = rhs.cap;
    rhs.elements = rhs.first_free = rhs.cap = nullptr;
  }
  return *this;
}

std::pair<std::string *, std::string *> StrVec::alloc_n_copy(const std::string *b, const std::string *e) {
  auto data = alloc.allocate(e - b);
  return {data, std::uninitialized_copy(b, e, data)};
}

void StrVec::push_back(const std::string &s) {
  chk_n_alloc();
  alloc.construct(first_free++, s);
}

// Note: 为什么要逆序
void StrVec::free() {
  if (elements) {
    for (auto p = first_free; p != elements;)
      alloc.destroy(--p);
    alloc.deallocate(elements, cap-elements);
  }
}

//void StrVec::reallocate() {
//  size_t newCap = !size() ? 1 : size() * 2;
//  auto newElement = alloc.allocate(newCap);
//  auto newFirstFree = newElement;
//  for(auto p = elements; p != first_free; p++) {
//    // Note: use std::move
//    alloc.construct(newFirstFree++, std::move(*p));
//  }
//  free();
//  elements = newElement;
//  first_free = newFirstFree;
//  cap = newElement + newCap;
//}

// 使用移动迭代器
void StrVec::reallocate() {
  size_t newCap = !size() ? 1 : size() * 2;
  auto newElement = alloc.allocate(newCap);
  auto newFirstFree = std::uninitialized_copy(
      std::make_move_iterator(begin()),
      std::make_move_iterator(end()),
      newElement
  );
  free();
  elements = newElement;
  first_free = newFirstFree;
  cap = newElement + newCap;
}
