//
// Created by skyitachi on 2020/1/31.
//

#include "concurrent_hashmap.h"
#include <gtest/gtest.h>

TEST(ConcurrentHashMap, Basic) {
  ConcurrentHashMap<int, int> cHashMap;
  cHashMap.put(1, 2);
  int a;
  bool found = cHashMap.get(1, &a);
  ASSERT_TRUE(found);
  ASSERT_TRUE(cHashMap.size() == 1);
  ASSERT_TRUE(cHashMap.containsKey(1));
  ASSERT_TRUE(!cHashMap.isEmpty());

  cHashMap.remove(1);
  ASSERT_TRUE(cHashMap.size() == 0);
  ASSERT_TRUE(!cHashMap.containsKey(1));
  ASSERT_TRUE(cHashMap.isEmpty());

}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}