//
// Created by skyitachi on 2019/11/1.
//

#include <cassert>
#include <rocksdb/db.h>
#include <rocksdb/write_batch.h>
#include <string>
#include <vector>
#include <iostream>

int main() {
  rocksdb::DB* db;
  rocksdb::Options options = rocksdb::Options();
  options.create_if_missing = true;
  rocksdb::Status status = rocksdb::DB::Open(options, "rocks_simple_db", &db);
  assert(status.ok());
  //single put
  for (int i = 0; i < 65536; i++) {
    db->Put(rocksdb::WriteOptions(), std::to_string(i), std::to_string(i));
  }
  // batch put, atomic write
  rocksdb::WriteBatch batch;
  batch.Delete("1");
  batch.Put("1", "2");
  status = db->Write(rocksdb::WriteOptions(), &batch);
  assert(status.ok());
  std::string value;
  status = db->Get(rocksdb::ReadOptions(), "1", &value);
  assert(status.ok());
  
  // not found
  status = db->Get(rocksdb::ReadOptions(), "none_exist", &value);
  assert(status.IsNotFound());
  
  // multi_get
  rocksdb::ReadOptions readOptions = rocksdb::ReadOptions();
  std::vector<std::string> results;
  std::vector<rocksdb::Slice> keys;
  std::vector<rocksdb::ColumnFamilyHandle*> column_families;
  keys.emplace_back("1");
  column_families.emplace_back(db->DefaultColumnFamily());
  keys.emplace_back("11");
  column_families.emplace_back(db->DefaultColumnFamily());
  // multi_get
  std::vector<rocksdb::Status> statuses = db->MultiGet(readOptions, column_families, keys, &results);
  for(auto status: statuses) {
    assert(status.ok());
  }
  for(auto v: results) {
    std::cout << v << std::endl;
  }
  // iterator, range api
  auto iterateOptions = rocksdb::ReadOptions();
  rocksdb::Slice start = "999";
  rocksdb::Slice end = "9991";
  iterateOptions.iterate_lower_bound = &start;
  iterateOptions.iterate_upper_bound = &end;
  auto it = db->NewIterator(iterateOptions);
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    std::cout << it->key().ToString() << ": " << it->value().ToString() << std::endl;
  }
  assert(it->status().ok());
  delete it;
  delete db;
}

