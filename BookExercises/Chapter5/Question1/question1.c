#include <stdio.h>

# ifdef _OPENMP
#include <omp.h>
# endif

int main(void){

# ifdef _OPENMP
	printf("_OPENMP value is: %d \n", _OPENMP);
# else
	printf("_OPENMP was not defined.");
# endif

	return 0;
}
