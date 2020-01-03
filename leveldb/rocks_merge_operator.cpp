//
// Created by skyitachi on 2020/1/2.
//

#include <rocksdb/db.h>
#include <string>
#include <memory>

class Counters {
public:
  // (re)set the value of a named counter
  virtual void Set(const std::string& key, uint64_t value);

  // remove the named counter
  virtual void Remove(const std::string& key);

  // retrieve the current value of the named counter, return false if not found
  virtual bool Get(const std::string& key, uint64_t *value);

  // increase the named counter by value.
  // if the counter does not exist,  treat it as if the counter was initialized to zero
  virtual void Add(const std::string& key, uint64_t value);
};

class RocksCounters : public Counters {
public:
  static uint64_t kDefaultCount = 0;
  RocksCounters(std::shared_ptr<rocksdb::DB> db): db_(db) {};

  // mapped to a RocksDB Put
  virtual void Set(const std::string& key, uint64_t value) {
    string serialized = Serialize(value);
    db_->Put(put_option_, key,  serialized));
  }

  // mapped to a RocksDB Delete
  virtual void Remove(const string& key) {
    db_->Delete(delete_option_, key);
  }

  // mapped to a RocksDB Get
  virtual bool Get(const std::string& key, uint64_t *value) {
    string str;
    auto s = db_->Get(get_option_, key,  &str);
    if (s.ok()) {
      *value = Deserialize(str);
      return true;
    } else {
      return false;
    }
  }

  // implemented as get -> modify -> set
  virtual void Add(const std::string& key, uint64_t value) {
    uint64_t base;
    if (!Get(key, &base)) {
      base = kDefaultValue;
    }
    Set(key, base + value);
  }

private:
  std::shared_ptr<rocksdb::DB> db_;
};

int main() {

}

