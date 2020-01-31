//
// Created by skyitachi on 2020/1/20.
//

#ifndef LEARNCPP_LOCK_FREE_STACK_H
#define LEARNCPP_LOCK_FREE_STACK_H
#include <atomic>
template <typename T>
class LockFreeStack {
private:
  struct node {
    T data;
    node *next;
    node(T const &data_): data(data_) {}
  };

  std::atomic<node*> head;
public:

  void push(T const& data) {
    node *new_node = new node(data);
    new_node->next = head.load();
    while(!head.compare_exchange_weak(new_node->next, new_node));
  }

  std::shared_ptr<T> pop() {
    node *old = head.load();
    // NOTE: 判断条件一定要放在循环里
    while(old != nullptr && !head.compare_exchange_weak(old, old->next));
    return old != nullptr ? old->data : std::shared<T>();
  }

};
#endif //LEARNCPP_LOCK_FREE_STACK_H
