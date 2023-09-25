#include <stdio.h>
#include <iostream>
#include <chrono>

#include "bitonic_sort.cpp"

int main() {
	auto trials = 5;

	std::cout << "n,time\n";
	for (size_t n : {131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608}) {
		auto *x = new size_t[n];

		for (auto trial=0; trial < trials; trial++) {

			rand_size_t_vec(x, n);
			const auto start{std::chrono::steady_clock::now()};
			bitonic_sort(x, n);
			const auto duration{std::chrono::steady_clock::now() -start};

			if (!is_ascending(x, n)) {
				throw std::invalid_argument("not sorted");
			}

			std::cout << n << "," << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "\n";
		}

		delete[] x;
	}
}
