#include <stdio.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <cmath>
#include <omp.h>
#include <time.h>

using namespace std;

#define DEBUG

void random_assngm(double *a, int length, int thread_count)
{
  int i;

# pragma omp parallel num_threads(thread_count) private(i)
{
  double *b=a;
  bool set=0;
# pragma omp for schedule(static)
  for (i = 0; i < length; i++) {
    if(!set){
      set=true;
      for(int k=0;k<i;k++){
        b++;
      }
    }
    *b++ = i;
  }
}
}

void Matrix_mult(float *A, float *B, float *res, int M, int L, int N, int thread_count)
{
  /* THIS FUNCTION ASSUMES: A[M][L] x B[L][N] = RES[M][N] */
  int i, j, k;
  for (i = 0; i < M; i++){
      for (j = 0; j < N; j++) {
          *res = 0;
          for (k = 0; k < L; k++) {
              *res += (*(A + i*L + k))*(*(B + k*N + j));
          }
          res++;
      }
  }
}


void print_matrix(double a[], int len)
{
  int i;
  for (i = 0; i < len; i++){
    cout << a[i] << " ";
  }
  cout << endl;
}

void Usage(){
  cout << "Error: Incorrect Arguments \n";
  cout << "Usage: ./matrixMult <DIMENSOIN> <thread_count> \n";
  exit(0);
}



int main(int argc, char const *argv[]) {

  int M, thread_count;

  M            = strtol(argv[1], NULL, 10);
  thread_count = strtol(argv[2], NULL, 10);


  double A[M];


  random_assngm(&A[0], M, thread_count);

  print_matrix(A, M);

  return 0;
}
