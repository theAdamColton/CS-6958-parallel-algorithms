#include <chrono>
#include <stdlib.h>
#include <iostream>
#include <omp.h>
#include "util.cpp"

int par_dense_matvec(double *M, double *x, double *y, int n, int m) {
#pragma omp parallel for
    for (int i = 0; i < n; i++) {
        double sum = 0.0;  // the dot product sum

#pragma omp parallel reduction(+ : sum)
        for (int j = 0; j < m; j++) {
            sum += M[i * m + j] * x[j];
        }
        y[i] = sum;
    }

    return 0;
}

int main() {
	std::cout << "time_seq,time_par,m,n\n";

	for (const int m : {2048 * 2}) {
		for (const int n : {512, 1024, 2048, 2048*2, 2048 * 4, 2048 * 8, 2048 * 16})  {
			const int trials = 8;

			double* M = new double[m*n];
			double* x = new double[m];
			double* y = new double[n];
			double* y2 = new double[n];

			for (int i = 0; i < trials; i++) {
				rand_vec(M, m*n);
				rand_vec(x, m);
				std::fill(y, y+n, 0.0);
				std::fill(y2, y2+n, 0.0);

				const auto start{std::chrono::steady_clock::now()};
				dense_matvec(M, x, y, n, m);
				const auto duration{std::chrono::steady_clock::now()-start};


				const auto start_par{std::chrono::steady_clock::now()};
				par_dense_matvec(M, x, y2, n, m);
				const auto duration_par{std::chrono::steady_clock::now()-start_par};

				
				bool result = vec_compare(y, y2, n);
				if (result == 0) {
					print_vec(y, n);
					print_vec(y2, n);
					throw std::runtime_error{"trial par not the same"};
				}

				std::cout << duration.count() << "," << duration_par.count() << "," << m << "," << n << "\n";
			}

			delete[] M;
			delete[] x;
			delete[] y;
			delete[] y2;
		}
	}

}
