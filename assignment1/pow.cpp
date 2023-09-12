#include <cmath>
#include <functional>
#include <iostream>
#include <omp.h>
#include <vector>

double pwr(double x, int cnt) { return cnt < 2 ? x : x * pwr(x, cnt - 1); }

int main() {
	int n = 100000;
	int p = omp_get_max_threads();
	std::vector<double> a(n), b(n);
	double dx = 1e-4;

	// some arbitrary decision using n and p.
	int chunksize = (n / p > 16) ? 16 : 1;
#pragma omp parallel
	{
#pragma omp for schedule(static, chunksize = 2048)
		for (int i = 0; i < n; ++i) {
			a[i] = sin(2 * M_PI * i * dx); // EW (a), CR(dx)
		}				       // for
#pragma omp for schedule(static, chunksize = 2048)
		for (int i = 0; i < n; ++i) {
			b[i] = pwr(a[i], i); // EW(b), ER(a)
		}			     // for
	}				     // omp parallel
} // main
