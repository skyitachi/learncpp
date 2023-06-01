//
// Created by Shiping Yao on 2023/6/1.
//
#include <iostream>
#include <chrono>
#include <folly/executors/GlobalExecutor.h>
#include <folly/io/async/EventBase.h>
#include <folly/futures/Future.h>
#include <folly/futures/Promise.h>
#include <thread>

using namespace std::chrono_literals;

folly::Future<int> doubleNumber(int x) {
  return folly::makeFuture(x * 2);
}

folly::Future<int> addNumbers(int x, int y) {
  return doubleNumber(x).thenValue([y](int doubled) {
    return y + doubled;
  });
}

int main() {
  folly::EventBase evb;
  auto executor = folly::getGlobalCPUExecutor();

  executor->add([&]() {
    std::cout << "task 1 start" << std::endl;
    std::this_thread::sleep_for(2s);
    std::cout << "task 1 done" << std::endl;
  });

  executor->add([&]() {
    std::cout << "task 2 start" << std::endl;
    std::this_thread::sleep_for(1s);
    std::cout << "task 2 done" << std::endl;
  });

  evb.loop();

  addNumbers(3, 4).thenValue([](int result) {
    std::cout << "result " << result << std::endl;
  });

  {
    folly::Promise<int> promise;
    folly::SemiFuture<int> semifuture = promise.getSemiFuture();

    // Register a callback to be executed when the semifuture is resolved.

    // Resolve the promise with a value.
    promise.setValue(10);

    // Wait for the semifuture to be resolved.
    semifuture.wait();

    std::cout << "semifuture value: " << semifuture.value() << std::endl;
  }
  return 0;
}
