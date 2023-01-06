//
// Created by skyitachi on 22-12-14.
//

#include <iostream>
#include <duckdb.hpp>
#include <mutex>
#include <future>

using namespace duckdb;

class SegmentLock {
public:
  SegmentLock() {}
  SegmentLock(std::mutex& mu): lock_(mu) {}
  SegmentLock(const SegmentLock &other) = delete;
  SegmentLock &operator=(const SegmentLock &) = delete;

  //! enable move constructors
  SegmentLock(SegmentLock &&other) noexcept {
    std::swap(lock_, other.lock_);
  }
  SegmentLock &operator=(SegmentLock &&other) noexcept {
    std::swap(lock_, other.lock_);
    return *this;
  }
private:
  std::unique_lock<std::mutex> lock_;
};

int main() {

  std::mutex g_mutex;

  int g_count = 0;

  std::vector<std::future<void>> futures;
  for (int i = 0; i < 10; i++) {
    auto f1 = std::async([&]{
      SegmentLock lock(g_mutex);
      for(int j = 0; j < 10000; j++) {
        g_count++;
      }
    });
    futures.push_back(std::move(f1));
  }
  for(auto& f: futures) {
    f.wait();
  }
  std::cout << "g_count: " << g_count << std::endl;

//  DBConfig config{};
//  config.file_system
//  DuckDB db("lineitem", &config);
//
//  Connection conn(db);
//
//  auto result = conn.Query("select count(*) from 'lineitem.parquet'");
//  result->Print();
}
