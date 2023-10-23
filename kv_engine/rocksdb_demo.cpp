//
// Created by skyitachi on 23-10-22.
//

#include <iostream>
#include <rocksdb/db.h>
#include <rocksdb/merge_operator.h>
#include <rocksdb/env.h>

using Slice = rocksdb::Slice;
using Logger = rocksdb::Logger;

bool Deserialize(Slice exist_value, uint64_t *existed) {
  if (exist_value.empty()) {
    return false;
  }
  std::memcpy((char *)existed, exist_value.data(), sizeof(uint64_t));
  return true;
}

std::string Serialize(uint64_t v) {
  std::string sv;
  sv.append(reinterpret_cast<char*>(&v), sizeof(uint64_t));
  return sv;
}

class UInt64AddOperator : public rocksdb::AssociativeMergeOperator {
public:

  virtual bool Merge(
      const Slice& key,
      const Slice* existing_value,
      const Slice& value,
      std::string* new_value,
      Logger* logger) const override {

    // assuming 0 if no existing value
    uint64_t existing = 0;
    if (existing_value) {
      if (!Deserialize(*existing_value, &existing)) {
        // if existing_value is corrupted, treat it as 0
        Log(logger, "existing value corruption");
        existing = 0;
      }
    }

    uint64_t oper;
    if (!Deserialize(value, &oper)) {
      // if operand is corrupted, treat it as 0
      Log(logger, "operand value corruption");
      oper = 0;
    }

    auto new_v = existing + oper;
    *new_value = Serialize(new_v);
    return true;        // always return true for this, since we treat all errors as "zero".
  }

  virtual const char* Name() const override {
    return "UInt64AddOperator";
  }
};

int main() {
  rocksdb::DB* db;
  rocksdb::Options options;
  options.merge_operator.reset(new UInt64AddOperator);
  options.create_if_missing = true;

  rocksdb::Status status = rocksdb::DB::Open(options, "rocksdb_demo_db", &db);
  if (!status.ok()) {
    std::cerr << "Failed to open database: " << status.ToString() << std::endl;
    return 1;
  }

  // 写入数据
  rocksdb::WriteOptions writeOptions;
  status = db->Put(writeOptions, "key1", "value1");
  if (!status.ok()) {
    std::cerr << "Failed to write data: " << status.ToString() << std::endl;
    delete db;
    return 1;
  }

  // 读取数据
  rocksdb::ReadOptions readOptions;
  std::string value;
  status = db->Get(readOptions, "key1", &value);
  if (status.ok()) {
    std::cout << "Retrieved value: " << value << std::endl;
  } else {
    std::cerr << "Failed to read data: " << status.ToString() << std::endl;
    delete db;
    return 1;
  }

  rocksdb::WriteOptions merge_options;
  std::string v1 = Serialize(1);
  status = db->Merge(merge_options, "c1", v1);
  assert(status.ok());

  std::string v2 = Serialize(2);
  status = db->Merge(merge_options, "c1", v2);
  assert(status.ok());


  std::string v3;
  status = db->Get(readOptions, "c1", &v3);
  if (status.ok()) {
    uint64_t iv3;
    std::cout << "counter：" << Deserialize(v3, &iv3) << " " << iv3 << std::endl;
  } else {
    std::cerr << "get counter failed: " << status.ToString() << std::endl;
  }

  delete db;
  return 0;
}