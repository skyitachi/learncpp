//
// Created by skyitachi on 2020/1/31.
//
#include "concurrent_hashmap.h"
#include <functional>

template <typename K, typename V>
void ConcurrentHashMap<K, V>::put(K key, V value) {
  std::size_t hash_ = std::hash<K>{}(key);
  size_t buckets = hash_ % segments_.size();
  segments_[buckets].set(key, value);
}

template<typename K, typename V>
void ConcurrentHashMap<K, V>::bucket::set(K key, V value) {
  std::lock_guard<std::mutex> lock(mu_);
  stores_.insert({key, value});
}
