//
// Created by skyitachi on 2019/12/31.
//

//
// Created by skyitachi on 2019/11/4.
//

#include <rocksdb/db.h>
#include <rocksdb/utilities/transaction.h>
#include <rocksdb/utilities/optimistic_transaction_db.h>
#include <rocksdb/utilities/transaction_db.h>
#include <iostream>

const std::string kDBPath = "rocks_test_db2";

void test_write_conflicts(rocksdb::OptimisticTransactionDB* txn_db) {
  rocksdb::Status s;
  auto writeOptions = rocksdb::WriteOptions();
  auto readOptions = rocksdb::ReadOptions();
  auto txn = txn_db->BeginTransaction(writeOptions);
  std::string value;

  s = txn->GetForUpdate(readOptions, "key1", &value);
  if (!s.ok()) {
    std::cout << "error code: " << s.ToString() << std::endl;
    delete txn;
  }
  // put will timeout or blocks, note: must use txn_db
  s = txn_db->Put(rocksdb::WriteOptions(), "key1", "value1");
  assert(s.ok());
  s = txn->Commit();
  std::cout << "error code: " << s.ToString() << std::endl;
  assert(s.IsBusy());
  std::cout << "test_write_conflicts success\n";
  delete txn;
}

int main() {
  // NOTE: 打开一个已经存在过的db，会有问题
  rocksdb::Options options = rocksdb::Options();
  options.create_if_missing = true;
  rocksdb::OptimisticTransactionOptions txn_db_options = rocksdb::OptimisticTransactionOptions();
  rocksdb::DB* db;
  rocksdb::OptimisticTransactionDB* txn_db;
//  txn_db_options.rollback_merge_operands = false;
  rocksdb::Status status = rocksdb::OptimisticTransactionDB::Open(options, kDBPath, &txn_db);
  std::cout << "status: " << status.ToString() << std::endl;
  assert(status.ok());

  rocksdb::WriteOptions writeOptions = rocksdb::WriteOptions();

  db = txn_db->GetBaseDB();
  rocksdb::Transaction* txn;

  // Snapshot，防止在事务开始后的任何与事务有想到key的写入
  txn = txn_db->BeginTransaction(rocksdb::WriteOptions());
  txn->SetSnapshot();
  db->Put(writeOptions, "key1", "value0");
  status = txn->Put("key1", "value1");
  // 这里不会冲突
  assert(!status.IsBusy());
  status = txn->Commit();
  // 这里才会出错
  assert(status.IsBusy());
  txn->Rollback();
  delete txn;

  test_write_conflicts(txn_db);
}

