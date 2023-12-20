//
// Created by skyitachi on 12/20/23.
//
#include <iostream>

#include <part/art.h>

using namespace part;
int main() {
  ArenaAllocator arena_allocator(Allocator::DefaultAllocator(), 16384);

  ART art;

  auto k1 = ARTKey::CreateARTKey<int64_t>(arena_allocator, 1);
  art.Put(k1, 1);

  std::vector<idx_t> results;

  bool success = art.Get(k1, results);
  if (success) {
    std::cout << "found doc id: " << results[0] << std::endl;
  }

}