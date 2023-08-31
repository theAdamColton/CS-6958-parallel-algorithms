#include <iostream>
#include <omp.h> 

int main() {

	int n = 10000000;
	int *array =  (int*) malloc (sizeof(int)*n);

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
    }


  }

  for (int i = 0; i < n; i++)
	  std::cout << array[i] << "\n";

  free(array);
}

