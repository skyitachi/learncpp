//
// Created by skyitachi on 2019-09-30.
//
#include <stdio.h>
#include <leveldb/db.h>
#include <iostream>

int main() {
  leveldb::DB *db;
  leveldb::Options options;
  options.compression = leveldb::CompressionType::kNoCompression;
  options.create_if_missing = true;
  options.reuse_logs = true;
  
  leveldb::Status status = leveldb::DB::Open(options, "test_level_db", &db);
  std::cout << "compression: " << options.compression << std::endl;
  if (!status.ok()) {
    std::cerr << "Unable to open/create test database './test_level_db'" << std::endl;
    std::cerr << status.ToString() << std::endl;
    return -1;
  }

  leveldb::WriteOptions writeOptions;
//  for (int i = 0; i < 1; i++) {
//    db->Put(writeOptions, "k" + std::to_string(i), "v" + std::to_string(i));
//  }
//  leveldb::Iterator *it = db->NewIterator(leveldb::ReadOptions());
//  for(it->SeekToFirst(); it->Valid(); it->Next()) {
//    std::cout << it->key().ToString() << ":" << it->value().ToString() << std::endl;
//  }
//  delete it;
  delete db;
}

