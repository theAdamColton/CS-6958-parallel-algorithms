#include "vector"
#include <iostream>
#include <omp.h>

/*
 * 0 0 0 0 0
 * 0 0 0 0 0
 * 0 0 0 0 0
 * 0 2 4 0 0
 * 0 0 3 0 0
 * 0 0 0 2 1
 * val = [2, 4, 3, 2, 1]
 * col = [1, 2, 2, 3, 4]
 * row_to_val = [0, 0, 0, 0, 2, 3]
 *
 *
 * 0 0 0 1 0 1
 * 0 2 0 0 0 0
 * 0 0 0 0 0 0
 * 0 0 0 0 0 0
 *
 * val = [1, 1, 2]
 * col = [3, 5, 1]
 * row_to_val = [0, 2, 4, 4]
 *
 * 0 0 0 0 0
 * 0 0 1 2 3
 * 0 0 0 0 0
 *
 * val = [1, 2, 3]
 * col = [2, 3, 4]
 * row_to_val = [0, 0, 3]
 *
 * 0 5 0
 * 0 0 0
 * 0 5 0
 * val = [5, 5]
 * col = [1, 1]
 * row_to_val = [0, 1, 1]
 */

struct CsrMatrix {

	// number of non zeros
	size_t nnz;
	// num rows
	size_t n;

	// num columns
	size_t m;

	// length = nnz
	double *val;

	// length = nnz
	size_t *col;

	// length = number of rows
	// stores the indices into val and col that indicate where a new row
	// starts
	//
	// if there are no non zero elements in row_to_val[i], then
	// row_to_val[i+1] == row_to_val[i]
	//
	// row_to_val can point to nnz, which is a fictitious element at the end
	int *row_to_val;

	CsrMatrix(size_t nnz, size_t n, size_t m, double *val, size_t *col,
		  int *row_to_val)
	    : nnz(nnz), n(n), m(m), val(val), col(col), row_to_val(row_to_val) {
	}

	~CsrMatrix() {
		delete[] val;
		delete[] col;
		delete[] row_to_val;
	}

	double *to_dense() const {
		double *out = new double[this->m * this->n]{0.0};

		// iterate over rows
		for (int row = 0; row < this->n; row++) {
			int val_start_pointer = this->row_to_val[row];

			// M has values in this row from val_start_pointer to
			// val_end_pointer
			int val_end_pointer;
			if (row + 1 == this->n) {
				val_end_pointer = nnz;
			} else {
				val_end_pointer = this->row_to_val[row + 1];
			}

			for (int val_pointer = val_start_pointer;
			     val_pointer < val_end_pointer; val_pointer++) {
				int col = this->col[val_pointer];
				// std::cout << "row: " << row << "  col: " <<
				// col << "  val: " << this->val[val_pointer] <<
				// " val_p: " << val_pointer << "\n";
				out[row * this->m + col] =
				    this->val[val_pointer];
			}
		}

		return out;
	}

	double *matmult_par(const double *x) const {
		double *out = new double[this->n];

		// iterate over rows
#pragma omp parallel for
		for (int row = 0; row < this->n; row++) {
			int val_start_pointer = this->row_to_val[row];

			// M has values in this row from val_start_pointer to
			// val_end_pointer
			int val_end_pointer;
			if (row + 1 == this->n) {
				val_end_pointer = nnz;
			} else {
				val_end_pointer = this->row_to_val[row + 1];
			}

			double sum = 0.0;
#pragma omp parallel reduction(+ : sum)
			for (int val_pointer = val_start_pointer;
			     val_pointer < val_end_pointer; val_pointer++) {
				int col = this->col[val_pointer];
				double val = this->val[val_pointer];
				double prod = x[col] * val;
				sum += prod;
			}

			out[row] = sum;
		}

		return out;
	}

	double *matmult(const double *x) const {
		double *out = new double[this->n];

		// iterate over rows
		for (int row = 0; row < this->n; row++) {
			int val_start_pointer = this->row_to_val[row];

			// M has values in this row from val_start_pointer to
			// val_end_pointer
			int val_end_pointer;
			if (row + 1 == this->n) {
				val_end_pointer = nnz;
			} else {
				val_end_pointer = this->row_to_val[row + 1];
			}

			double sum = 0.0;
			for (int val_pointer = val_start_pointer;
			     val_pointer < val_end_pointer; val_pointer++) {
				int col = this->col[val_pointer];
				double val = this->val[val_pointer];
				double prod = x[col] * val;
				sum += prod;
			}

			out[row] = sum;
		}

		return out;
	}

	static CsrMatrix from_dense(const double *M, size_t n, size_t m) {
		std::vector<double> csr_val_vec{};
		std::vector<size_t> csr_col_vec{};

		int *csr_row_to_val = new int[n];

		for (size_t row = 0; row < n; row++) {
			int val_i_start = csr_val_vec.size();

			for (size_t col = 0; col < m; col++) {
				double val = M[row * m + col];

				if (val == 0.) {
					continue;
				}

				csr_val_vec.push_back(val);
				csr_col_vec.push_back(col);
			}

			int val_pointer;
			// if there's nothing in this row
			if (val_i_start == csr_val_vec.size()) {
				val_pointer = -1;
			} else {
				// points to the first added element
				val_pointer = val_i_start;
			}

			csr_row_to_val[row] = val_pointer;
		}

		size_t nnz = csr_val_vec.size();

		// Backwards pass
		for (int row = n - 1; row > -1; row--) {
			size_t val_pointer = csr_row_to_val[row];
			if (val_pointer == -1) {
				if (row == n - 1) {
					csr_row_to_val[row] = nnz;
				} else {
					csr_row_to_val[row] =
					    csr_row_to_val[row + 1];
				}
			}
		}

		double *csr_val = new double[nnz];
		size_t *csr_col = new size_t[nnz];

		for (int i = 0; i < nnz; i++) {
			csr_val[i] = csr_val_vec[i];
			csr_col[i] = csr_col_vec[i];
		}

		return CsrMatrix(nnz, n, m, csr_val, csr_col, csr_row_to_val);
	}
};
