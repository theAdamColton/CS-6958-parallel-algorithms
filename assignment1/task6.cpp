#include <chrono>
#include <iostream>
#include <random>

#include "util.cpp"
#include "csr.cpp"

void random_zero(double *m, size_t n, double p) {
	for (int i = 0; i < n; i++) {
		if (rand_0_1() < p) {
			m[i] = 0.0;
		}
	}
}

int main() {
	std::cout << "time,m,n,p\n";

	double p = 0.2;

	for (const int m : {1024}) {
		for (const int n : {32})  {
			const int trials = 1024 * 1024 * 100;

			for (int i = 0; i < trials; i++) {

				double* M = new double[m*n];
				double* x = new double[m];

				rand_vec(M, m*n);
				random_zero(M, m*n, p);
				rand_vec(x, m);

				auto M_csr = CsrMatrix::from_dense(M, n, m);
				auto M_from_csr = M_csr.to_dense();

				auto res = vec_compare(M, M_from_csr, n*m);

				if (res == false) {
					print_vec(M, m*n);
					print_vec(M_from_csr, m*n);
					print_vec(M_csr.val, M_csr.nnz);
					print_vec(M_csr.col, M_csr.nnz);
					print_vec(M_csr.row_to_val, M_csr.n);
				}

				delete[] M;
				delete[] x;
				delete[] M_from_csr;
			}
		}
	}
}
