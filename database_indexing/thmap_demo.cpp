//
// Created by skyitachi on 24-1-22.
//
#include <cstdint>
#include <atomic>
#include <thread>
#include <vector>
#include <iostream>

#include "thmap/thmap.h"

constexpr uint64_t LOCK_FLAG = 0x1;

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

int main() {
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