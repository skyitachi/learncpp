//
// Created by skyitachi on 2020/1/10.
//

#include <memory>
#include <mutex>
template <typename T>
class Queue {
private:
  struct node {
    std::shared_ptr<T> data;
    std::unique_ptr<node> next;
    node(T data_): data(std::move(data_)){};
  };
  std::unique_ptr<node> head;
  node* tail;
  std::mutex head_mutex;
  std::mutex tail_mutex;

  node* get_tail() {
    std::lock_guard tail_lock(tail_mutex);
    return tail;
  }

  std::unique_ptr<node> pop_head() {
    std::lock_guard head_lock(head_mutex);
    if (head.get() == get_tail()) {
      return nullptr;
    }
    std::unique_ptr<node> old_head = std::move(head);
    head = std::move(old_head->next);
    return old_head;
  }

public:
  // 保证队列至少有一个节点
  Queue(): head(new node), tail(head.get()) {}
  Queue(const Queue& other)=delete;
  Queue&operator =(const Queue& other)= delete;

  std::shared_ptr<T> try_pop() {
    std::unique_ptr old_head = pop_head();
    return old_head ? old_head->data : std::shared_ptr<T>();
  }

  void push(T new_value) {
    std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
    std::unique_ptr<node> p(new node);
    node* const new_tail = p.get();
    std::lock_guard<std::mutex> tail_lock(tail_mutex);
    tail->data = new_data;
    tail->next = std::move(p);
    tail = new_tail;
  }
};