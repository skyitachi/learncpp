//
// Created by skyitachi on 2020/1/31.
//

#ifndef LEARNCPP_CONCURRENT_HASHMAP_H
#define LEARNCPP_CONCURRENT_HASHMAP_H
#include <shared_mutex>
#include <vector>
#include <map>
#include <functional>

template <typename K, typename V, typename Hash = std::hash<K>>
class ConcurrentHashMap {
private:
  struct bucket {

    void set(K key, V value) {
      std::lock_guard<std::shared_mutex> lock(mu_);
      stores_.insert({key, value});
    }

    bool get(const K& key, V* vptr) {
      std::shared_lock<std::shared_mutex> lock(mu_);
      auto it = stores_.find(key);
      if (it != stores_.end()) {
        *vptr = it->second;
        return true;
      }
      return false;
    }

    void remove(const K& key) {
      std::lock_guard<std::shared_mutex> lock(mu_);
      stores_.erase(key);
    }

    bool containsKey(const K& key) {
      std::shared_lock<std::shared_mutex> lock(mu_);
      return stores_.find(key) != stores_.end();
    }

    size_t size() {
      std::shared_lock<std::shared_mutex> lock(mu_);
      return stores_.size();
    }

  private:
    std::shared_mutex mu_;
    std::map<K, V> stores_;
  };

  bucket& get_bucket(K const &key) const {
    std::size_t idx = hasher_(key) % segments_.size();
    return *segments_[idx];
  }
  Hash hasher_;
  std::vector<std::unique_ptr<bucket>> segments_;
  int buckets_size_;

public:
  ConcurrentHashMap(): buckets_size_(16), hasher_(Hash()) {
    for(int i = 0; i < buckets_size_; i++) {
      segments_.emplace_back(new bucket);
    }
  }

  void put(K key, V value) {
    get_bucket(key).set(key, value);
  };

  void remove(const K& key) {
    get_bucket(key).remove(key);
  }

  bool containsKey(const K &key) const {
    return get_bucket(key).containsKey(key);
  }

  bool isEmpty() const {
    return size() == 0;
  };

  bool get(const K& key, V* vptr) {
    return get_bucket(key).get(key, vptr);
  };

  size_t size() const {
    size_t ret = 0;
    for (auto &segment: segments_) {
      ret += segment->size();
    }
    return ret;
  }
};

#endif //LEARNCPP_CONCURRENT_HASHMAP_H
