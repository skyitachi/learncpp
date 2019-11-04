//
// Created by skyitachi on 2019/11/4.
//
#include <rocksdb/db.h>
#include <iostream>
#include <string>
#include <cassert>

const std::string kDBPath = "rocks_test_db";

class NumericComparator: public rocksdb::Comparator {
public:
  int Compare(const rocksdb::Slice& a, const rocksdb::Slice& b) const override {
//    std::cout << "key1 " << a.ToString() << " key2 " << b.ToString() << std::endl;
    try {
      int v1 = std::stoi(a.ToString());
      int v2 = std::stoi(b.ToString());
      if (v1 < v2) {
        return -1;
      } else if (v1 > v2) {
        return 1;
      }
      return 0;
    } catch (std::invalid_argument& e) {
      return 0;
    }
  }
  const char* Name() const override {
    return "NumericComparator";
  }
  void FindShortestSeparator(std::string *, const rocksdb::Slice &) const override {};
  
  void FindShortSuccessor(std::string *) const override {};
};

int main() {
  rocksdb::DB* db;
  rocksdb::Options options = rocksdb::Options();
  options.create_if_missing = true;
  NumericComparator numericComparator;
  options.comparator = &numericComparator;
  
  rocksdb::Status status = rocksdb::DB::Open(options, kDBPath, &db);
  assert(status.ok());
  auto it = db->NewIterator(rocksdb::ReadOptions());
  it->SeekToFirst();
  std::cout << "smallest value: " << it->value().ToString() << std::endl;
  
}
