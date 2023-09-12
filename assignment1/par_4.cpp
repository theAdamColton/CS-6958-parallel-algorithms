#include <iostream>
#include <omp.h>

int main() {
	int n = 16;
#pragma omp parallel for
	for (int i = 0; i < n; ++i) {
		int rank = omp_get_thread_num(); // rank is a local variable
		std::cout << "Hello #" << i << " from thread " << rank
			  << std::endl;
	}
}
