//
// Created by skyitachi on 2020/1/3.
//

#include <rocksdb/db.h>
#include <titan/db.h>

int main() {
  rocksdb::titandb::TitanDB *db;
  using rocksdb::titandb::TitanOptions;
  rocksdb::Status s = rocksdb::titandb::TitanDB::Open(TitanOptions(), "titandb", &db);
  assert(s.ok());
  using rocksdb::WriteOptions;
  s = db->Put(WriteOptions(), "key1", "value1");
  assert(s.ok());

  using rocksdb::ReadOptions;
  std::string value;
  db->Get(ReadOptions(), "key1", &value);
  delete db;
}
