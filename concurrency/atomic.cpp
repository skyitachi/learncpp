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
  // barrier
  std::atomic_thread_fence(std::memory_order_release);
  y.store(true, std::memory_order_relaxed);
}

void read_y_then_x() {
  while(!y.load(std::memory_order_relaxed));
  std::atomic_thread_fence(std::memory_order_acquire);
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

// 5.9
std::atomic<int> gData[5];
std::atomic<bool> sync1(false), sync2(false);

std::atomic<int> syncCAS(0);

void thread_1() {
  gData[0].store(1, std::memory_order_relaxed);
  gData[1].store(2, std::memory_order_relaxed);
  gData[2].store(3, std::memory_order_relaxed);
  gData[3].store(4, std::memory_order_relaxed);
  gData[4].store(5, std::memory_order_relaxed);
  sync1.store(true, std::memory_order_release);
  
  // cas style
//  syncCAS.store(1, std::memory_order_release);
}

void thread_2() {
  while(!sync1.load(std::memory_order_acquire));
  sync2.store(true, std::memory_order_release);
  // cas style
//  int expected = 1;
//  while(!syncCAS.compare_exchange_strong(expected, 2, std::memory_order_acq_rel)) {
//    expected = 1;
//  }
}

void thread_3() {
  while(!sync2.load(std::memory_order_acquire));
  
  // cas style
//  while(!syncCAS.load(std::memory_order_acquire) < 2);
  assert(gData[0].load(std::memory_order_relaxed) == 1);
  assert(gData[1].load(std::memory_order_relaxed) == 2);
  assert(gData[2].load(std::memory_order_relaxed) == 3);
  assert(gData[3].load(std::memory_order_relaxed) == 4);
  assert(gData[4].load(std::memory_order_relaxed) == 5);
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

