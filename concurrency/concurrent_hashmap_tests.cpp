//
// Created by skyitachi on 2020/1/31.
//

#include "concurrent_hashmap.h"

int main() {
  auto cHashMap = ConcurrentHashMap<int, int>();
  cHashMap.put(1, 2);
}