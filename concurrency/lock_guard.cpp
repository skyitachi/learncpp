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

int main() {
  std::cout << "main: " << g_i << std::endl;
  std::thread t1(safe_increment);
  std::thread t2(safe_increment);
  t1.join();
  t2.join();
  std::cout << "main: " << g_i << std::endl;
}
