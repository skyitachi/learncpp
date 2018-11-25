//
// Created by skyitachi on 2018/11/25.
//

#include <future>
#include <thread>
#include <iostream>

int main() {
  std::packaged_task<int()> task([] { return 7;});
  std::future<int> f1 = task.get_future();
  std::thread t(std::move(task));

  std::future<int> f2 = std::async(std::launch::async, []{return 8;});

  std::promise<int> p;
  std::future<int> f3 = p.get_future();
  std::thread([&p]{p.set_value_at_thread_exit(9);}).detach();

  std::cout << "waiting ..." << std::endl;
  f1.wait();
  f2.wait();
  f3.wait();

  std::cout << "Done!\nResults are: "
            << f1.get() << ' ' << f2.get() << ' ' << f3.get() << '\n';
  t.join();
}

