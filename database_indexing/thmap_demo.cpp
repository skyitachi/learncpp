//
// Created by skyitachi on 24-1-22.
//
#include <cstdint>
#include <thread>
#include <vector>
#include <iostream>

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



int main() {

  return 0;
}