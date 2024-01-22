//
// Created by skyitachi on 24-1-22.
//
#include <cstdint>
#include <atomic>
#include <thread>
#include <vector>
#include <iostream>
#include <type_traits>

#include "thmap/thmap.h"

constexpr uint64_t LOCK_FLAG = 0x1;

constexpr uint64_t SHARED_LOCK_FLAG = 0x2;

#define	SPINLOCK_BACKOFF_MIN	4
#define	SPINLOCK_BACKOFF_MAX	128
#if defined(__x86_64__) || defined(__i386__)
#define SPINLOCK_BACKOFF_HOOK	__asm volatile("pause" ::: "memory")
#else
#define SPINLOCK_BACKOFF_HOOK
#endif
#define	SPINLOCK_BACKOFF(count) \
do {								\
	int __i;						\
	for (__i = (count); __i != 0; __i--) {			\
		SPINLOCK_BACKOFF_HOOK;				\
	}							\
	if ((count) < SPINLOCK_BACKOFF_MAX)			\
		(count) += (count);				\
} while (/* CONSTCOND */ 0);

void lock(std::atomic<uint64_t>& node) {
    unsigned bcount = 4;
    uint64_t s;
again:
    s = std::atomic_load(&node);
    if (s & LOCK_FLAG) {
        SPINLOCK_BACKOFF(bcount);
        goto again;
    }
    /* Acquire from prior release in unlock_node.() */
    if (!node.compare_exchange_weak(s, s | LOCK_FLAG)) {
        bcount = SPINLOCK_BACKOFF_MIN;
        goto again;
    }
}

void unlock(std::atomic<uint64_t>  &node) {
  uint64_t s = node.load(std::memory_order_relaxed) & ~LOCK_FLAG;

  node.store(s, std::memory_order_release);
}

class Node {
public:
  Node(uint64_t data): data_(data) {}

public:
  uint64_t data_;
  uint64_t lock_ = {0};
};

void lock_node(std::atomic<Node> &node) {

  unsigned bcount = 4;
  Node s = Node(0);
  again:
  s = std::atomic_load(&node);
  if (s.data_ & LOCK_FLAG) {
    SPINLOCK_BACKOFF(bcount);
    goto again;
  }
  /* Acquire from prior release in unlock_node.() */
  Node newS(s.data_ | LOCK_FLAG);
  if (!node.compare_exchange_weak(s, newS)) {
    bcount = SPINLOCK_BACKOFF_MIN;
    goto again;
  }

}

void unlock_node(std::atomic<Node>  &node) {
  uint64_t s = node.load(std::memory_order_relaxed).data_ & ~LOCK_FLAG;

  node.store(Node(s), std::memory_order_release);
}

void shared_lock_node(std::atomic<Node> &node) {
  do {
    Node n = node.load(std::memory_order_acquire);
    if (!(n.lock_ & LOCK_FLAG)) {
      break;
    }
  } while(true);

}

void lock_demo() {
  std::atomic<uint64_t> node = {0};
  int g_count = 0;

  std::cout << " sizeof(std::atomic<uint64_t>) = " << sizeof(node) <<std::endl;
  std::cout << " sizeof(std::atomic<uint32_t>) = " << sizeof(std::atomic<uint32_t>) <<std::endl;

  std::vector<std::thread> ths;

  for (int i = 0; i < 10; i++) {
    ths.emplace_back([&] {
      for (int i = 0; i < 10000; i++) {
        lock(node);
        g_count++;
        unlock(node);
      }
    });

  }

  for(auto &th: ths) {
    th.join();
  }

  std::cout << "final count: " <<g_count << std::endl;

}

void lock_node_demo() {
  std::atomic<Node> node = {0};
  int g_count = 0;

  std::vector<std::thread> ths;

  for (int i = 0; i < 10; i++) {
    ths.emplace_back([&] {
      for (int i = 0; i < 10000; i++) {
        lock_node(node);
        g_count++;
        unlock_node(node);
      }
    });

  }

  for(auto &th: ths) {
    th.join();
  }

  std::cout << "lock by node final count: " <<g_count << std::endl;
}

int main() {

  lock_node_demo();

  auto ret = std::is_trivially_copyable<Node>::value;

  std::cout << "trivially_copyable: " << ret << std::endl;
  ret = std::is_copy_constructible<Node>::value;
  std::cout << "is_copy_constructible: " << ret << std::endl;
  ret = std::is_move_constructible<Node>::value;
  std::cout << "is_move_constructible: " << ret << std::endl;
  ret = std::is_copy_assignable<Node>::value;
  std::cout << "is_copy_assignable: " << ret << std::endl;
  ret = std::is_move_assignable<Node>::value;

  std::cout << "is_move_assignable: " << ret << std::endl;

  return 0;
}