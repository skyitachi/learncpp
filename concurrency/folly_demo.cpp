//
// Created by Shiping Yao on 2022/5/23.
//

#include <iostream>
#include <folly/futures/Future.h>
#include <folly/executors/ThreadedExecutor.h>


void foo(int x) {
    std::cout << "foo(" << x << ")" << std::endl;
}

int main() {
    folly::ThreadedExecutor executor;
    folly::Promise<int> p;
    folly::Future<int> f = p.getSemiFuture().via(&executor);
    auto f2 = std::move(f).thenValue(foo);
    std::cout << "Future chain made" << std::endl;

    p.setValue(10);

    std::move(f2).get();
    std::cout << "Promise fulfilled" << std::endl;
}
