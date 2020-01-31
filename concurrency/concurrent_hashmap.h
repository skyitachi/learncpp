//
// Created by skyitachi on 2020/1/31.
//

#ifndef LEARNCPP_CONCURRENT_HASHMAP_H
#define LEARNCPP_CONCURRENT_HASHMAP_H
#include <mutex>
#include <vector>
#include <map>

template <typename K, typename V>
class ConcurrentHashMap {
private:
  struct bucket {
    void set(K key, V value);
  private:
    std::mutex mu_;
    std::map<K, V> stores_;
  };
  std::vector<bucket> segments_;
  int buckets_size_;
public:
  ConcurrentHashMap(): buckets_size_(16) {
    for(int i = 0; i < buckets_size_; i++) {
      segments_.emplace_back(bucket{});
    }
  }
  void put(K key, V value);
  bool containsKey(K key);
  bool isEmpty();
  V get(K key);
  size_t size();
};

#endif //LEARNCPP_CONCURRENT_HASHMAP_H
