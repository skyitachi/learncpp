//
// Created by skyitachi on 22-12-14.
//

#include <iostream>
#include <duckdb.hpp>

using namespace duckdb;

int main() {
  DBConfig config{};
//  config.file_system
  DuckDB db("lineitem", &config);

  Connection conn(db);

  auto result = conn.Query("select count(*) from 'lineitem.parquet'");
  result->Print();
}
