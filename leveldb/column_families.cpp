//
// Created by skyitachi on 2019/11/8.
//
#include <rocksdb/db.h>
#include <string>
#include <cassert>
#include <iostream>

int main() {
  // open db with column families
  rocksdb::DB* db;
  rocksdb::Options options = rocksdb::Options();
  options.create_if_missing = true;
  std::vector<rocksdb::ColumnFamilyDescriptor> columnFamilyDescriptor;
  std::vector<rocksdb::ColumnFamilyHandle*> columnFamilyHandle;
  // 有一个default column_family
  columnFamilyDescriptor.emplace_back(rocksdb::kDefaultColumnFamilyName, rocksdb::ColumnFamilyOptions());
  columnFamilyDescriptor.emplace_back("new_cf", rocksdb::ColumnFamilyOptions());
  rocksdb::Status status = rocksdb::DB::Open(options, "rocks_test_db", columnFamilyDescriptor, &columnFamilyHandle, &db);
  std::cout << "status: " << status.ToString() << std::endl;
  assert(status.ok());
  

  for (int i = 0; i < 65536 * 2; i++) {
    db->Put(rocksdb::WriteOptions(), columnFamilyHandle[0], std::to_string(i), std::to_string(i));
    db->Put(rocksdb::WriteOptions(), columnFamilyHandle[1], std::to_string(i), std::to_string(i));
  }
  assert(status.ok());
  
  for(auto handle: columnFamilyHandle) {
    delete handle;
  }
  
  delete db;
//  rocksdb::ColumnFamilyHandle* cf;
//  status = db->CreateColumnFamily(rocksdb::ColumnFamilyOptions(), "new_cf", &cf);
//  assert(status.ok());
//
//  status = db->Put(rocksdb::WriteOptions(), cf, "p1", "v1");
//  assert(status.ok());
//
//  status = db->Put(rocksdb::WriteOptions(), nullptr, "p", "v");
//  assert(status.ok());
  
}

