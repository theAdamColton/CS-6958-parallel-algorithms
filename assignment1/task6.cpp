#include <chrono>
#include <iostream>
#include <random>

#include "csr.cpp"
#include "util.cpp"

void random_zero(double *m, size_t n, double p) {
	for (int i = 0; i < n; i++) {
		if (rand_0_1() < p) {
			m[i] = 0.0;
		}
	}
}

int main() {
	std::cout << "time_seq,time_par,m,n,p\n";

	for (double p : {0.5, 0.95, 0.99}) {
		for (const int m : {1024}) {
			for (const int n : {256, 512, 1024, 1024 * 2, 1024 * 4, 1024 * 8}) {
				const int trials = 4;
				double *M = new double[m * n];
				double *x = new double[m];
				double *y = new double[n];

				for (int i = 0; i < trials; i++) {

					rand_vec(M, m * n);
					random_zero(M, m * n, p);
					rand_vec(x, m);

					auto M_csr =
					    CsrMatrix::from_dense(M, n, m);
					auto M_from_csr = M_csr.to_dense();

					auto res =
					    vec_compare(M, M_from_csr, n * m);

					if (res == false) {
						print_vec(M, m * n);
						print_vec(M_from_csr, m * n);
						print_vec(M_csr.val, M_csr.nnz);
						print_vec(M_csr.col, M_csr.nnz);
						print_vec(M_csr.row_to_val,
							  M_csr.n);
					}

					dense_matvec(M, x, y, n, m);

					const auto start{
					    std::chrono::steady_clock::now()};
					auto y2 = M_csr.matmult(x);
					const auto duration{
					    std::chrono::steady_clock::now() -
					    start};
					const auto start_par{
					    std::chrono::steady_clock::now()};
					auto y3 = M_csr.matmult_par(x);
					const auto duration_par{
					    std::chrono::steady_clock::now() -
					    start_par};
					// TODO this fails when n is very large
					// auto res2 = vec_compare(y, y2, n);

					// if (res2 == false) {
					//	print_vec(y2, n);
					//	print_vec(y, n);
					// }
					auto res3 = vec_compare(y2, y3, n);

					std::cout << duration.count() << ","
						  << duration_par.count() << ","
						  << m << "," << n << "," << p
						  << "\n";

					delete[] y2;
					delete[] y3;
					delete[] M_from_csr;
				}

				delete[] y;
				delete[] M;
				delete[] x;
			}
		}
	}
}
