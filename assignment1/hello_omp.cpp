#include <iostream>
#include <omp.h> 

int main() {

	int n = 100;
	int *array =  (int*) malloc (sizeof(int)*n);

#pragma omp parallel
  {
    int size;
    unsigned short rank;
    size = omp_get_num_threads();
    rank = omp_get_thread_num();
    auto seed = seed48(&rank);

    for (int i = rank; i < n; i += size) {
	    array[i] = rand();
    }


  }

  for (int i = 0; i < n; i++)
	  std::cout << array[i] << "\n";

  free(array);
}

