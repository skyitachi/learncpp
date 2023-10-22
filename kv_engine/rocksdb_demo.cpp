//
// Created by skyitachi on 23-10-22.
//

#include <iostream>
#include <rocksdb/db.h>

int main() {
  rocksdb::DB* db;
  rocksdb::Options options;
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

  delete db;
  return 0;
}