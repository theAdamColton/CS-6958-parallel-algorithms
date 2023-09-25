#include <iostream>
#include <random>

template <typename T> void print_vec(T *x, int n) {
	std::cout << "vec: ";

	for (int i = 0; i < n; i++)
		std::cout << x[i] << ",";
	std::cout << "\n";
}

void rand_size_t_vec(size_t *x, size_t n) {
	for (int i = 0; i < n; i++ ) {
		x[i] = rand();
	}
}

bool is_ascending(size_t *x, size_t n) {
	for (int i = 0; i < n-1; i++) {
		if (x[i] > x[i+1]) {
			return false;
		}
	}
	return true;
}
