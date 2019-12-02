//
// Created by skyitachi on 2019/12/2.
//
#include <iostream>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>

bool p1 = false;
bool p2 = false;

int main() {
  std::condition_variable cv;
  std::mutex m;
  std::thread t1([&m, &cv]{
    std::unique_lock<std::mutex> lk(m);
    p1 = true;
    std::cout << "thread 1 set" << std::endl;
    lk.unlock();
    cv.notify_one();
  });
  std::thread t2([&m, &cv]{
    std::unique_lock<std::mutex> lk(m);
    p2 = true;
    std::cout << "thread 2 set" << std::endl;
    lk.unlock();
    cv.notify_one();
  });
  std::unique_lock<std::mutex> lk(m);
  cv.wait(lk, []{
    std::cout << "cv wait 1" << std::endl;
    return p1;
  });
  std::cout << "first wait done" << std::endl;
  cv.wait(lk, []{
    std::cout << "cv wait 2" << std::endl;
    return p2;
  });
  std::cout << "second wait done" << std::endl;
  t1.join();
  t2.join();
  
  std::atomic<int *> newest_writer(nullptr);
  int *ptr = newest_writer.load();
  if (ptr == nullptr) {
    std::cout << "just right\n";
  }
  return 0;
}

