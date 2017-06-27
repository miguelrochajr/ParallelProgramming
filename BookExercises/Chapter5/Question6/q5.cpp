/*
  Compile: g++ -g -Wall -fopenmp -o q5 q5.cpp

  Author: Miguel Silva da Rocha Jr.
  Source Code taken from: https://github.com/miguelrochajr
  Email: miguelsrochajr@gmail.com

*/

 #include <iostream>
 #include <omp.h>
 #include <cstdlib>
 #include <stdio.h>
 #include <stdlib.h>

 void Usage(char prog_name[]);
 void Get_args(char* argv[], int* thread_count, int* n);
 void Schedule(int thread_count, int n, int max[], int min[]);
 void PrintResult(int max[], int min[], int thread_count, int n);

 int main(int argc, char* argv[]) {
    if (argc != 3) Usage(argv[0]);

    int thread_count, n;
    Get_args(argv, &thread_count, &n); // Initialize the values above

    int max[thread_count];
    int min[thread_count];

    Schedule(thread_count, n, max, min);
    PrintResult(max,min, thread_count, n);

    return 0;
 }

 void PrintResult(int max[], int min[], int thread_count, int n){
   for (int i = 0; i < thread_count; i++) {
      if (min[i] == n && max[i] == 0) /* This can happen when thread_count>n */
         printf("Thread %d: No iterations\n", i);
      else if (min[i] != max[i])
         printf("Thread %d: Iterations %d - %d\n", i, min[i], max[i]);
      else
         printf("Th %d > Iteration  %d\n", i, min[i]);
   }
 }

 void Usage(char prog_name[]) {
    fprintf(stderr, "usage: %s <thread_count> <n>\n", prog_name);
    exit(0);
 }

 void Get_args(char* argv[], int* thread_count, int* n) {
    *thread_count = strtol(argv[1], NULL, 10);
    *n = strtol(argv[2], NULL, 10);
 }

 void Schedule(int thread_count, int n, int max[], int min[]) {
    int  i;

    for (i = 0; i < thread_count; i++) {
       min[i] = n;
       max[i] = 0;
    }

 #  pragma omp parallel num_threads(thread_count) \
       default(none) private(i) shared(min, max, n)
    {
    int my_rank = omp_get_thread_num();
 #     pragma omp for
       for (i = 0; i < n; i++) {
 #        ifdef DEBUG
          printf("Th %d > iteration %d\n", my_rank, i);
 #        endif
          if (i < min[my_rank])
             min[my_rank] = i;
          if (i > max[my_rank])
             max[my_rank] = i;
       }
    }
 }
