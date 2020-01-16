//
// Created by skyitachi on 2018/11/25.
//
#include <thread>
#include <mutex>
#include <iostream>

int g_i = 0;
std::mutex g_i_mutex;

void safe_increment() {
  // RAII style
  std::lock_guard<std::mutex> lock(g_i_mutex);
  ++g_i;
  std::cout << std::this_thread::get_id() << ": " << g_i << std::endl;
}

void test_lock() {
  std::mutex mu1;
  std::mutex mu2;
  std::lock(mu1, mu2);
  std::cout << "acquire lock successfully\n";
  std::lock_guard<std::mutex> lock_1(mu1, std::adopt_lock);
  std::lock_guard<std::mutex> lock_2(mu1, std::adopt_lock);
  try {
    // dead lock
    std::lock(mu1, mu2); // 用于锁定2个 mutex
    std::cout << "acquire lock twice\n";
  } catch (std::exception e) {
    std::cout << e.what() << std::endl;
  }
}

void test_recursive_lock() {
  std::recursive_mutex mu1;
  std::recursive_mutex mu2;

  std::lock(mu1, mu2);
  std::cout << "acquire recursive mutex successfully\n";
  std::lock_guard<std::recursive_mutex> lock_1(mu1, std::adopt_lock);
  std::lock_guard<std::recursive_mutex> lock_2(mu1, std::adopt_lock);
  try {
    // no dead lock
    std::lock(mu1, mu2);
    std::cout << "acquire recursive mutex twice\n";

  } catch (std::exception e) {
    std::cout << e.what() << std::endl;
  }

}

void test_unique_lock() {
  std::mutex mu;
  int c = 0;
  std::thread t1([&mu, &c] {
    std::unique_lock lk(mu);
    assert(lk.owns_lock());
    c += 1;

  });

  std::thread t2([&mu, &c] {
    std::unique_lock lk(mu);
    assert(lk.owns_lock());
    c += 1;
  });
  t1.join();
  t2.join();
  assert(c == 2);
}

int main() {
  std::cout << "main: " << g_i << std::endl;
  std::thread t1(safe_increment);
  std::thread t2(safe_increment);
  t1.join();
  t2.join();
  std::cout << "main: " << g_i << std::endl;
//  test_lock();
  test_recursive_lock();
  test_unique_lock();
}
