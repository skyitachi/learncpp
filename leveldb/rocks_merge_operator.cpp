//
// Created by skyitachi on 2020/1/2.
//

#include <rocksdb/db.h>
#include <rocksdb/merge_operator.h>
#include <memory>
#include <string>
#include <memory>

using namespace rocksdb;
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

class MyAppender: public AssociativeMergeOperator {
public:
  virtual ~MyAppender() {}

  virtual bool Merge(const Slice& key, const Slice* existing_value, const Slice& value, std::string *new_value, Logger* logger) const {
    if (existing_value) {
      printf("existing value: %s, value: %s\n", existing_value->data(), value.data());
      *new_value = existing_value->ToString() + value.ToString();
    } else {
      *new_value = value.ToString();
    }
    return true;
  }

  virtual const char *Name() const {
    return "MyAppender";
  }
};

int main() {
  rocksdb::DB* db;
  rocksdb::Options options = rocksdb::Options();
  options.create_if_missing = true;
  options.merge_operator = std::make_shared<MyAppender>();
  rocksdb::Status status = rocksdb::DB::Open(options, "rocks_merge_db", &db);
  auto ro = ReadOptions();

  auto wo = WriteOptions();
  db->Merge(wo, "counter", "a");
  std::string current_value;
  auto s = db->Get(ro, "counter", &current_value);
  assert(s.ok());
  printf("current value: %s\n", current_value.c_str());
  db->Merge(wo , "counter", "b");
  s = db ->Get(ro, "counter", &current_value);
  assert(s.ok());
  printf("current value: %s\n", current_value.c_str());
  return 0;
}

