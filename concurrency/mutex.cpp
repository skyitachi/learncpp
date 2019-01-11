//
// Created by skyitachi on 2019/1/4.
//

#include <boost/asio/thread_pool.hpp>
#include <boost/asio.hpp>
#include <mutex>
#include <thread>
#include <stdio.h>
#include <vector>

class MutexObj {

public:
  void Add() {
    mu_.lock();
    counter_ += 1;
    mu_.unlock();
  }
  int Value() {
    return counter_;
  }
private:
  std::mutex mu_;
  int counter_;
};

class Wrapper {
public:
  Wrapper() {
    moList_.push_back(new MutexObj());
    moList_.push_back(new MutexObj());
  }

  void Operate(int i) {
    if (i % 2 == 1) {
      moList_[1]->Add();
    } else {
      moList_[0]->Add();
    }
  }

private:
  std::vector<MutexObj *> moList_;
};

int main() {
  boost::asio::thread_pool pool(64);
  Wrapper* wrapper = new Wrapper();
  for(int i = 0; i < 100000; i++) {
    boost::asio::post(pool, [i, wrapper]{
      wrapper->Operate(i);
    });
  }
  pool.join();
}
