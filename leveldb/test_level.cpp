//
// Created by skyitachi on 2019-09-30.
//
#include <stdio.h>
#include <leveldb/db.h>
#include <iostream>

int main() {
  leveldb::DB *db;
  leveldb::Options options;
  options.create_if_missing = true;
  leveldb::Status status = leveldb::DB::Open(options, "./test.db", &db);
  if (!status.ok()) {
    std::cerr << "Unable to open/create test database './testdb'" << std::endl;
    std::cerr << status.ToString() << std::endl;
    return -1;
  }
  leveldb::WriteOptions writeOptions;
  db->Put(writeOptions, "a", "b");
  
  leveldb::Iterator *it = db->NewIterator(leveldb::ReadOptions());
  for(it->SeekToFirst(); it->Valid(); it->Next()) {
    std::cout << it->key().ToString() << ":" << it->value().ToString() << std::endl;
  }
}

