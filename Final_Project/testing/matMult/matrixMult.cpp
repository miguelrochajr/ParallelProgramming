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

void Matrix_mult(float A[], float B[], float res[],
  int M, int L, int N, int thread_count)
{
  /* THIS FUNCTION ASSUMES: A[M][L] x B[L][N] = RES[M][N] */
  int i, j, k;
  double result = 0;
/* Creates the team of threads */
# pragma omp parallel num_threads(thread_count) \
   private(i, j, k) shared(A,B, res,M,L,N) reduction (+: result)
{
# pragma omp for schedule(static) /* Use the defautt scheduling */
  for (i = 0; i < M; i++){
      for (j = 0; j < N; j++) {
          result = 0;
          for (k = 0; k < L; k++) {
              result+= A[k + i * L] * B[j + k * N];
          }
          res[j + i * N] = (float)result; /*REMINDER: result is a Double variable*/
      }
  }
}
} /* End of mult */

// void Matrix_mult(float *Ax, float *Bx, float *resx, int M, int L, int N, int thread_count)
// {
//   /* THIS FUNCTION ASSUMES: A[M][L] x B[L][N] = RES[M][N] */
//
// # pragma omp parallel num_threads(thread_count)
// {
//   int i, j, k;
//   float *A  = Ax;
//   float *B  = Bx;
//   float *res= resx;
//   bool  set = 0;
// # pragma omp for schedule(static)
//   for (i = 0; i < M; i++)
//   {
//       if(!set)
//       {
//         set=true;
//         for(int k=0;k<i;k++)
//         {
//           A++;
//           res++;
//         }
//       }
//       for (j = 0; j < N; j++)
//       {
//           *res = 0;
//           for (k = 0; k < L; k++)
//           {
//               *res += (*(A + i*L + k))*(*(B + k*N + j));
//           }
//           res++;
//       }
//   }
// }
// } /* End of matrix multiplication */

void random_assngm(float a[], int length)
{
  srand(time(NULL));
  for (int i = 0; i < length; i++) {
    #ifdef DEBUG
      a[i] = i;
    #else
      a[i] = (rand()%100)/100.0;
    #endif
  }
}

void print_matrix(float a[], int rows, int cols)
{
  int j,i;
  for (i = 0; i < rows; i++){
    for (j = 0; j < cols; j++){
      cout << a[i*cols + j] << " ";
    }
    cout << endl;
  }
  cout << "-----------" << endl;
}

void Usage(){
  cout << "Error: Incorrect Arguments \n";
  cout << "This program assumes the following multiplication: \n \n";
  cout << "       A[M][L] x B[L][N] = RES[M][N] \n \n";
  cout << "Usage: ./matrixMult <M> <L> <N> <thread_count> \n";
  exit(0);
}

void Activation_func(double in[],double out[],int rows, int cols,
                    int thread_count) /*  SIGMOID ACTIVATION FUNCTION */
{
  int i,j;
# pragma omp parallel num_threads(thread_count) \
     private(i, j) shared(in, out,rows, cols)
  {
#   pragma omp for schedule(static) /* Use the defautt scheduling */
  	for(i=0;i < rows;i++)
  	{
  		for(j = 0;j< cols;j++)
  		{
  			out[i*cols+j] = 1.0/(1.0 + exp(-(in[i*cols+j])));
  		}
  	}
  }
}


int main(int argc, char const *argv[]) {

  int M,  L,  N, thread_count;
  # ifdef DEBUG
    M = 2;
    L = 4;
    N = 3;
    thread_count = strtol(argv[1], NULL, 10);
  # else
    if(argc < 5) Usage();
    M            = strtol(argv[1], NULL, 10);
    L            = strtol(argv[2], NULL, 10);
    N            = strtol(argv[3], NULL, 10);
    thread_count = strtol(argv[4], NULL, 10);
  # endif

  float A[M*L], B[L*N], res[M*N];
  float expectedResult[] = { 42, 48, 54, 114, 136, 158};
  double start, finish;

  random_assngm(A, M*L);
  random_assngm(B, L*N);

  # ifdef DEBUG
    print_matrix(A, M, L);
    print_matrix(B, L, N);
  # endif


  start = omp_get_wtime();
  Matrix_mult(A, B, res, M, L, N, thread_count);
  finish = omp_get_wtime();

  cout << "Total time taken: " << finish-start << "\n \n" ;

  # ifdef DEBUG
    print_matrix(res, M, N);
    cout << " Expected matrix result is: \n \n";
    print_matrix(expectedResult, 2, 3);
  # endif

  return 0;
}
