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
  int b=0;
# pragma omp parallel for num_threads(thread_count) private(b, i)
  for (i = 0; i < length; i++) {
      //if(omp_get_thread_num() == 0)
        cout << "Thread " << omp_get_thread_num() << " b:" << b << endl;
      b++;
  }
}

void print_matrix(double a[], int rows, int cols)
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
  cout << "Usage: ./matrixMult <DIMENSOIN> <thread_count> \n";
  exit(0);
}



int main(int argc, char const *argv[]) {

  int M, thread_count;

  M            = strtol(argv[1], NULL, 10);
  thread_count = strtol(argv[2], NULL, 10);


  double A[M*M];


  random_assngm(&A[0], M*M, thread_count);

  print_matrix(A, M, M);

  return 0;
}
