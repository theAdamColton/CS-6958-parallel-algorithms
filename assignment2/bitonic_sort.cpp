#include <iostream>
#include <stdio.h>
#include <omp.h>

#include "util.cpp"


template <typename T>
void swap(T *r, T *s)
{
   T tmp = *r;
   *r = *s;
   *s = tmp;
} 

template <typename T>
void make_bitonic(T *x, size_t blocksize, size_t jmp, size_t j) {
	for (size_t k = 0; k < jmp; k++) {
		bool asc = ((j+k) / (blocksize * 2)) % 2 == 0;
		//print_vec(x, n);
		// compare x[j + k] and x[j + k + jmp]
		if (asc) {
			if (x[j+k] > x[j+k+jmp]) {
				swap(&x[j+k], &x[j+k+jmp]);
			}
		} else {
			if (x[j+k] < x[j+k+jmp]) {
				swap(&x[j+k], &x[j+k+jmp]);
			}
		}
		//std::cout << "bs: " << blocksize << ", jmp: " << jmp << ", j: " << j << ", k: " << k << ", asc: " << asc << "\n";
		//print_vec(x, n);
		//std::cout << "\n";
	}
}

template <typename T>
void make_bitonic_parallel(T *x, size_t blocksize, size_t jmp, size_t j) {
#pragma omp parallel for schedule(static, 64)
	for (size_t k = 0; k < jmp; k++) {
		bool asc = ((j+k) / (blocksize * 2)) % 2 == 0;
		//print_vec(x, n);
		// compare x[j + k] and x[j + k + jmp]
		if (asc) {
			if (x[j+k] > x[j+k+jmp]) {
				swap(&x[j+k], &x[j+k+jmp]);
			}
		} else {
			if (x[j+k] < x[j+k+jmp]) {
				swap(&x[j+k], &x[j+k+jmp]);
			}
		}
		//std::cout << "bs: " << blocksize << ", jmp: " << jmp << ", j: " << j << ", k: " << k << ", asc: " << asc << "\n";
		//print_vec(x, n);
		//std::cout << "\n";
	}
}

/*
 * step: the size of each in-order list is 2^step
 *
 * x: is a bitonic series formed of 2^step sized arrays that are increasing, decreasing, increasing, decreasing, ...
 *
 *
 * This is what x looks like at bsort(x, n, 1)
 *  /\  /\  /\  /\
 * /  \/  \/  \/  \
 *
 *
 *
 */
template <typename T>
void bitonic_sort(T *x, size_t n) {
	for (size_t blocksize = 1; blocksize <= n / 2; blocksize *= 2) {
		// for i = [blocksize, blocksize / 2, ..., 2]
		for (size_t jmp = blocksize; jmp >= 1; jmp /= 2) {
#pragma omp parallel for 
			for (size_t j = 0; j < n; j += jmp * 2) {
				if (jmp > 1024 ) {
					make_bitonic_parallel(x, blocksize, jmp, j);
				} else {
					make_bitonic(x, blocksize, jmp, j);
				}
			}

		}
	}
}

