#include <iostream>
#include <random>

double rand_0_1() { return ((double)rand() / (RAND_MAX)); }

bool vec_compare(double *x, double *y, int n) {
	double eps = 1e-3;
	for (int i = 0; i < n; i++) {
		double diff = fabs(x[i] - y[i]);
		if (diff > eps) {
			std::cout << "Not equal! " << x[i] << "," << y[i]
				  << ", diff = " << diff << " @ " << i << "\n";
			return false;
		}
	}
	return true;
}

double *rand_vec(double *x, int n) {
	std::random_device r;
	std::default_random_engine re(r());
	std::normal_distribution<double> unif;

	for (int i = 0; i < n; i++) {
		x[i] = unif(re);
	}
	return x;
}

template <typename T> void print_vec(T *x, int n) {
	std::cout << "vec: ";

	for (int i = 0; i < n; i++)
		std::cout << x[i] << ",";
	std::cout << "\n";
}

void dense_matvec(double *M, double *x, double *y, int n, int m) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			y[i] += M[i * m + j] * x[j];
		}
	}
}
