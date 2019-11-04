//
// Created by skyitachi on 2019/11/4.
//

#include <rocksdb/db.h>
#include <rocksdb/utilities/transaction.h>
#include <rocksdb/utilities/optimistic_transaction_db.h>
#include <rocksdb/utilities/transaction_db.h>
#include <iostream>

const std::string kDBPath = "rocks_test_db1";

int main() {
  // NOTE: 打开一个已经存在过的db，会有问题
  rocksdb::Options options = rocksdb::Options();
  options.create_if_missing = true;
  rocksdb::TransactionDBOptions txn_db_options = rocksdb::TransactionDBOptions();
  rocksdb::DB* db;
  rocksdb::TransactionDB* txn_db;
//  txn_db_options.rollback_merge_operands = false;
  rocksdb::Status status = rocksdb::TransactionDB::Open(options, txn_db_options, kDBPath, &txn_db);
  std::cout << "status: " << status.ToString() << std::endl;
  assert(status.ok());
  
  rocksdb::WriteOptions writeOptions = rocksdb::WriteOptions();
  
  db = txn_db->GetBaseDB();
  rocksdb::Transaction* txn = txn_db->BeginTransaction(rocksdb::WriteOptions());
  status = txn->Put("10", "11");
  status = txn->Delete("key2");
  status = txn->Merge("key3", "value");
  status = txn->Commit();
  
  // Snapshot，防止在事务开始后的任何与事务有想到key的写入
  txn = txn_db->BeginTransaction(rocksdb::WriteOptions());
  txn->SetSnapshot();
  db->Put(writeOptions, "key1", "value0");
  status = txn->Put("key1", "value1");
  // 这里已经会冲突了
  assert(status.IsBusy());
  std::cout << "error code: " << status.code() << std::endl;
  txn->Rollback();
  delete txn;
}