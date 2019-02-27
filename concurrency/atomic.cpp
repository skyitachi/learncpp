//
// Created by skyitachi on 2019-02-27.
//

#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<bool> x, y;

std::atomic<int> z;

std::atomic<bool> ready(false);

int data = 0;

void write_x_then_y() {
  x.store(true, std::memory_order_relaxed);
  y.store(true, std::memory_order_relaxed);
}

void read_y_then_x() {
  while(!y.load(std::memory_order_relaxed));
  if(x.load(std::memory_order_relaxed))
    ++z;
}

// memory_order_release and acquire
void producer() {
  data = 10;
  ready.store(true, std::memory_order_release);
}

void consumer() {
  while(!ready.load(std::memory_order_acquire));
  assert(data == 10); // ensured
}

int main() {
  x = false;
  y = false;
  z = 0;
//  std::thread a(write_x_then_y);
//  std::thread b(read_y_then_x);
  std::thread a(producer);
  std::thread b(consumer);
  a.join();
  b.join();
//  assert(z.load() != 0); // 有可能被触发
}

