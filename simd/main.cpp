#include <iostream>

#include <benchmark/benchmark.h>
#include <emmintrin.h>

void add(int *x, int *y, int *z, int len) {
	__m128i *vecX = (__m128i*)x;
	__m128i *vecY = (__m128i*)y;
	__m128i *vecZ = (__m128i*)z;
	for (int i = 0; i < len / 4; i++) {
		_mm_store_si128(vecZ++, _mm_add_epi32(*vecX++, *vecY++));
	}
}

void naive_add(int *x, int *y, int *z, int len) {
  for(int i = 0; i < len; i++) {
    z[i] = x[i] + y[i];
  }
}

static void BM_SimdFunction(benchmark::State& state) {
  int x[] = {1, 2, 3, 4, 6, 7, 8, 9};
  int y[] = {2,3, 4, 5, 10, 11, 12, 9};
  int z[] = {0,0,0,0, 0, 0, 0, 0};
  for (auto _ : state) {
    add(x, y, z, 8);
  }
}

static void BM_NaiveFunction(benchmark::State& state) {
  int x[] = {1, 2, 3, 4, 6, 7, 8, 9};
  int y[] = {2,3, 4, 5, 10, 11, 12, 9};
  int z[] = {0,0,0,0, 0, 0, 0, 0};
  for (auto _ : state) {
    naive_add(x, y, z, 8);
  }
}

BENCHMARK(BM_SimdFunction);
BENCHMARK(BM_NaiveFunction);
BENCHMARK_MAIN();