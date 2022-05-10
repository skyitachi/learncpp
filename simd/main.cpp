#include <iostream>

#include <emmintrin.h>

void add(int *x, int *y, int *z, int len) {
	__m128i *vecX = (__m128i*)x;
	__m128i *vecY = (__m128i*)y;
	__m128i *vecZ = (__m128i*)z;
	for (int i = 0; i < len / 4; i++) {
		_mm_store_si128(vecZ++, _mm_add_epi32(*vecX++, *vecY++));
	}
}

int main() {
	int x[] = {1, 2, 3, 4};
	int y[] = {2,3, 4, 5};
	int z[] = {0,0,0,0};
	add(x, y, z, 4);
	for (int i =0; i < 4; i++) {
		std::cout << z[i] << " ";
	}
	std::cout << std::endl;
}