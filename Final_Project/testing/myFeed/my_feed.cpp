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

#define OUTPUTS	1
#define INPUTS  2
#define SAMPLES 1

void Random_assngm(double a[], int length);
void Matrix_mult(double A[], double B[], double res[],
                int M, int L, int N, int thread_count);
void Activation_func(double in[],double out[],
                int rows, int cols,int thread_count);
void Usage();
void Print_matrix(double a[], int rows, int cols);

int main(int argc, char const *argv[]) {

  if (argc < 2) Usage();

  int NEURONS, thread_count;
  double start, finish;
  NEURONS = strtol(argv[1], NULL, 10);
  thread_count = strtol(argv[2], NULL, 10);

  /*  Input vector */
  double X[(INPUTS+1)*SAMPLES] = {0,0};

  /* Hidden layer */
	double Wx[NEURONS*(INPUTS+1)];  // Hidden layer weights matrix
	double ILF1[NEURONS*SAMPLES];   // Induced Local field first layer
	double ATV1[NEURONS*SAMPLES];   // Activated induced local fields

  /* Output Layer */
  double Wy[OUTPUTS*(NEURONS+1)];  // Output layer weights matrix
  double ILF2[OUTPUTS*SAMPLES];   // Induced Local field second layer
  double ATV2[OUTPUTS*SAMPLES];   // Activated induced local fields

  /* Initialize Weights with random values */
  Random_assngm(Wx, NEURONS*(INPUTS+1));
  Random_assngm(Wy, OUTPUTS*(NEURONS+1));

  start = omp_get_wtime();

  Matrix_mult(Wx, X, ILF1, NEURONS,INPUTS+1,SAMPLES,  thread_count); // multiplies each input to each weight assigned to them
  Activation_func(ILF1, ATV1, NEURONS, SAMPLES, thread_count);

  Matrix_mult(Wy, ATV2, ILF2, OUTPUTS, NEURONS+1, SAMPLES, thread_count);
  Activation_func(ILF2, ATV2, OUTPUTS, SAMPLES, thread_count);

  finish = omp_get_wtime();


  cout << "Time taken: " << finish-start << endl;

  #ifdef DEBUG
    Print_matrix(ATV2, OUTPUTS, SAMPLES);
  #endif

  return 0;
}

void Usage()
{
	cout << "Some arguments are missing. \n";
	cout << "Usage: ./feed <NEURONS> <thread_count> \n";
  exit(0);
}

void Print_matrix(double a[], int rows, int cols)
{
  int j,i;
  for (i = 0; i < rows; i++){
    for (j = 0; j < cols; j++){
      cout << a[i*cols + j] << " ";
    }
    cout << endl;
  }
}

void Matrix_mult(double A[], double B[], double res[],
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
          res[j + i * N] = result; /*REMINDER: result is a Double variable*/
      }
  }
}
} /* End of mult */

void Random_assngm(double a[], int length)
{
  double random;
  srand(time(NULL));
  for (int i = 0; i < length; i++) {
    random = ((double)(rand()%10000))/10000;
    a[i] = random;
  }
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
