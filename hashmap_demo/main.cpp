//
// Created by skyitachi on 23-3-7.
//
#include <iostream>
#include "flat_hash_map.hpp"

int main() {
  ska::flat_hash_map<int, std::string> fmap;
  fmap.insert({1, "hello"});
  std::cout << fmap.size() << std::endl;
  fmap[100] = "world";

  auto v = fmap[2];
  std::cout << "after look for none exist values, fmap size: " << fmap.size() << std::endl;

  auto it  = fmap.find(2);
  if (it != fmap.end()) {
    std::cout << "after lookup none exist key by [] operator: it will auto insert" << std::endl;
  } else {
    std::cout << "it works fine" << std::endl;
  }
}