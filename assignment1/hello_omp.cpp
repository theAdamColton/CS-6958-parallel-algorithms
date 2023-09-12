#include <climits>
#include <cstdio>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

int main() {
	int n = 100000000;

	long *array = new long[n];

#pragma omp parallel
	{
		int size;
		unsigned short rank;
		size = omp_get_num_threads();
		rank = omp_get_thread_num();

		unsigned short seed[3];
		seed[0] = rank;
		seed[1] = rank + 1;
		seed[2] = rank + 2;

		for (int i = rank; i < n; i += size) {
			array[i] = nrand48(seed);
			// printf("%d\n", array[i]);
		}
	}

	delete[] array;
}
