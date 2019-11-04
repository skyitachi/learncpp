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
  
  leveldb::Status status = leveldb::DB::Open(options, "db", &db);
  std::cout << "compression: " << options.compression << std::endl;
  if (!status.ok()) {
    std::cerr << "Unable to open/create test database './testdb'" << std::endl;
    std::cerr << status.ToString() << std::endl;
    return -1;
  }
//  leveldb::WriteOptions writeOptions;
//  for (int i = 0; i < 6; i++) {
//    db->Put(writeOptions, std::to_string(i), std::to_string(i));
//  }
//  leveldb::Iterator *it = db->NewIterator(leveldb::ReadOptions());
//  for(it->SeekToFirst(); it->Valid(); it->Next()) {
//    std::cout << it->key().ToString() << ":" << it->value().ToString() << std::endl;
//  }
}

