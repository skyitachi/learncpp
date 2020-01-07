//
// Created by skyitachi on 2019/11/4.
//

#include <rocksdb/db.h>
#include <rocksdb/utilities/transaction.h>
#include <rocksdb/utilities/transaction_db.h>
#include <iostream>
#include <thread>

const std::string kDBPath = "rocks_test_db3";

void test_save_points(rocksdb::TransactionDB*);
// it's a PessimisticTransactionDB
void test_write_conflicts(rocksdb::TransactionDB* txn_db) {
  rocksdb::Status s;
  auto writeOptions = rocksdb::WriteOptions();
  auto readOptions = rocksdb::ReadOptions();
  auto txn = txn_db->BeginTransaction(writeOptions);
  std::string value;

  s = txn->GetForUpdate(readOptions, "key3", &value);
  assert(s.ok());
  // put will timeout or blocks, note: must use txn_db
  s = txn_db->Put(rocksdb::WriteOptions(), "key3", "value1");
  assert(s.IsTimedOut());
  txn->Rollback();
  std::cout << "test_write_conflicts success\n";
  delete txn;
}

void test_repeatable_read(rocksdb::TransactionDB* txn_db) {
  rocksdb::Status s;
  auto writeOptions = rocksdb::WriteOptions();
  auto readOptions = rocksdb::ReadOptions();
  auto db = txn_db->GetBaseDB();
  readOptions.snapshot = db->GetSnapshot();
  auto txn = txn_db->BeginTransaction(writeOptions);
  std::string value;

  s = txn->GetForUpdate(readOptions, "key3", &value);
  assert(s.ok());
  std::cout << "value is " << value << std::endl;
  s = db->Put(writeOptions, "key3", "value10");
  assert(s.ok());
  std::string value1;
  s = txn->GetForUpdate(readOptions, "key3", &value1);
  assert(value == value1);
  s = txn->Commit();
  assert(s.ok());
//  txn->Rollback();
  std::cout << "test_repeatable_read success\n";
  txn_db->ReleaseSnapshot(readOptions.snapshot);
  delete txn;
  txn_db->Get(readOptions, "key3", &value1);
  assert(value1 == "value10");
}

int main() {
  rocksdb::Options options = rocksdb::Options();
  options.create_if_missing = true;
  std::shared_ptr<rocksdb::Logger> logger;
  rocksdb::Env *env = rocksdb::Env::Default();
  env->NewLogger("rocksdb_.log", &logger);
  options.info_log = logger;

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
//  status = txn->Merge("key3", "value");
  status = txn->Commit();
  
  // Snapshot，防止在事务开始后的任何与事务有相同key的写入
  txn = txn_db->BeginTransaction(rocksdb::WriteOptions());
  txn->SetSnapshot();
  db->Put(writeOptions, "key1", "value0");
  status = txn->Put("key1", "value1");
  // 这里已经会冲突了
  assert(status.IsBusy());
  std::cout << "error code: " << status.code() << std::endl;
  txn->Rollback();
  delete txn;

  test_repeatable_read(txn_db);
  test_write_conflicts(txn_db);
  test_save_points(txn_db);
}

void test_save_points(rocksdb::TransactionDB* txn_db) {
  auto txn = txn_db->BeginTransaction(rocksdb::WriteOptions());
  auto s = txn->Put("A", "a");
  txn->SetSavePoint();
  s = txn->Put("B", "b");
  txn->RollbackToSavePoint();
  s = txn->Commit();

  std::string v;
  s = txn_db->Get(rocksdb::ReadOptions(), "A", &v);
  assert(s.ok());
  assert(v == "a");
  s = txn_db->Get(rocksdb::ReadOptions(), "B", &v);
  assert(s.IsNotFound());
  std::cout << "test_save_points works ok\n";
}

