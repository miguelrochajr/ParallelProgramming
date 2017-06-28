#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <cmath>
#include <omp.h>
#include <time.h>

using namespace std;


void assign(int *a, int len){
  int i;
  for (i = 0; i < len; i++) {
    *(a+i) = i;
  }
}

void print_Vect(int *a, int len){
  for (int i = 0; i < len; i++)
  {
    cout << "A[" << i << "]: " << *(a+i) << endl;
  }
}

int main(int argc, char const *argv[]) {
  int a[10];
  int len = 10;

  assign(&a[0], len);
  print_Vect(&a[0],len);


  return 0;
}
