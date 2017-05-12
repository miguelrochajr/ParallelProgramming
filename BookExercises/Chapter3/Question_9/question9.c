/*

Code written by Miguel Rocha Jr (miguelsrochajr@gmail.com) in order to complete the assignment-exam
for the course Concurrent and Distributed Programming, taught by Samuel Xavier.

These series of codes are my solutions manual for the book An Introduction to Parallel Programming by Peter Pacheco, 1st Edition.

Chapter 3
Exercise 3.9

Note:

1. The dimension of vector n should be evenly divisible by comm_sz
2. This program is based on mpi_vector_add.c, found under /ch3 folder in the book source materials


*/

#include <stdio.h>
#include <string.h>
#include <mpi.h>

void Check_for_error(int local_ok, char fname[], char mesasge[], MPI_Comm comm);
void Read_n(int* n_p, int* local_n_p, int my_rank, int comm_sz, MPI_Comm comm);
void Allocate_vectors(double** local_x_pp, double** local_y_pp,
        double** local_z_pp, int local_n, MPI_Comm comm);
void Read_vector(double local_a[], int local_n, int n, char vec_name[],
        int my_rank, MPI_Comm comm); //this functions reads in the vectors
void Print_vector(double local_b[], int local_n, int n, char title[],
        int my_rank, MPI_Comm comm);

/* functions implemented to do the dot product and a multiplication of a vector by a scalar*/

int main(void){
  int n, local_n;
  int comm_sz, my_rank;
  double *local_x, *local_y, *local_z; 
}
