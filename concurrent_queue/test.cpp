//
// Created by skyitachi on 2019-03-13.
//

#include "blockingconcurrentqueue.h"
#include <thread>
#include <chrono>
#include <assert.h>

int main() {
  moodycamel::BlockingConcurrentQueue<int> queue;
  std::thread producer([&]() {
    for(int i = 0; i != 100; ++i) {
      std::this_thread::sleep_for(std::chrono::milliseconds(i % 10));
      queue.enqueue(i);
    }
  });
  
  std::thread consumer([&]{
    for(int i = 0; i < 100; i++) {
      int item;
      queue.wait_dequeue(item);
      assert(item == i);
      if (queue.wait_dequeue_timed(item, std::chrono::milliseconds(5))) {
        ++i;
        assert(item == i);
      }
    }
  });
  
  producer.join();
  consumer.join();
  assert(queue.size_approx() == 0);
}

