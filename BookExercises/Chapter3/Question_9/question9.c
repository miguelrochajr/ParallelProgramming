#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

void Read_n(int* n_p, int* local_n_p, int my_rank, int comm_sz,
      MPI_Comm comm);
void Read_data(double local_vec1[], double local_vec2[], double* scalar_p,
      int local_n, int my_rank, int comm_sz, MPI_Comm comm);
void Print_vector(double local_vec[], int local_n, int n, char title[],
      int my_rank, MPI_Comm comm);
double Perform_dot_product(double local_vec1[], double local_vec2[],
      int local_n, MPI_Comm comm);
void Par_vector_scalar_mult(double local_vec[], double scalar,
      double local_result[], int local_n);

int main(void) {
   int n, local_n;
   double *local_vec1, *local_vec2;
   double scalar;
   double *local_scalar_mult1, *local_scalar_mult2;
   double dot_product;
   int comm_sz, my_rank;
   MPI_Comm comm;

   MPI_Init(NULL, NULL);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &comm_sz);
   MPI_Comm_rank(comm, &my_rank);

   Read_n(&n, &local_n, my_rank, comm_sz, comm);
   /*
    para
    comm_sz = 4
    n = 16
    local_n = 16/4 = 4
   */

   local_vec1 = malloc(local_n*sizeof(double));
   local_vec2 = malloc(local_n*sizeof(double));
   local_scalar_mult1 = malloc(local_n*sizeof(double));
   local_scalar_mult2 = malloc(local_n*sizeof(double));

   Read_data(local_vec1, local_vec2, &scalar, local_n, my_rank, comm_sz, comm); // Here everybdy has everything

   dot_product = Perform_dot_product(local_vec1, local_vec2, local_n, comm);
   if (my_rank == 0) {
      printf("Dot product is %f\n", dot_product);
   }

   Par_vector_scalar_mult(local_vec1, scalar, local_scalar_mult1, local_n);
   Par_vector_scalar_mult(local_vec2, scalar, local_scalar_mult2, local_n);

   Print_vector(local_scalar_mult1, local_n, n,
         "The product of the first vector with scalar is",
         my_rank, comm);
   Print_vector(local_scalar_mult2, local_n, n,
         "The product of the second vector with scalar is",
         my_rank, comm);

   free(local_scalar_mult2);
   free(local_scalar_mult1);
   free(local_vec2);
   free(local_vec1);

   MPI_Finalize();
   return 0;
}

void Read_n(int* n_p, int* local_n_p, int my_rank, int comm_sz,
      MPI_Comm comm) {
   int local_ok = 1;

   if (my_rank == 0){
      printf("Please type the vector order. \n");
      scanf("%d", n_p);
   }
   MPI_Bcast(n_p, 1, MPI_INT, 0, comm);
   *local_n_p = *n_p / comm_sz;
}

void Read_data(double local_vec1[], double local_vec2[], double* scalar_p,
      int local_n, int my_rank, int comm_sz, MPI_Comm comm) {
   double* aux = NULL;
   int i;
   if (my_rank == 0){
      printf("What is the scalar?\n");
      scanf("%lf", scalar_p);
   }

   MPI_Bcast(scalar_p, 1, MPI_DOUBLE, 0, comm);

   if (my_rank == 0){
      aux = malloc(local_n * comm_sz * sizeof(double));
      printf("Enter the first vector\n");
      for (i = 0; i < local_n * comm_sz; i++)
         scanf("%lf", &aux[i]);
      MPI_Scatter(aux, local_n, MPI_DOUBLE, local_vec1, local_n,
            MPI_DOUBLE, 0, comm);
      printf("Enter the second vector\n");
      for (i = 0; i < local_n * comm_sz; i++)
         scanf("%lf", &aux[i]);
      MPI_Scatter(aux, local_n, MPI_DOUBLE, local_vec2, /* Sender part. This part is most important to the sender */
        local_n, MPI_DOUBLE, 0, comm);
      free(aux);
   } else {
      MPI_Scatter(aux, local_n, MPI_DOUBLE, /* Send part. Is important for the sender but not to the receiver, which is the case here */
          local_vec1, local_n, MPI_DOUBLE, 0, comm); /* Receive part. This is what really matters here. */
      MPI_Scatter(aux, local_n, MPI_DOUBLE, local_vec2, local_n,
            MPI_DOUBLE, 0, comm);
   }
}

void Print_vector(double local_vec[], int local_n, int n, char title[],
      int my_rank, MPI_Comm comm) {
   double* a = NULL;
   int i;

   if (my_rank == 0) {
      a = malloc(n * sizeof(double));
      MPI_Gather(local_vec, local_n, MPI_DOUBLE, a, local_n,
            MPI_DOUBLE, 0, comm);
      printf("%s\n", title);
      for (i = 0; i < n; i++)
         printf("%.2f ", a[i]);
      printf("\n");
      free(a);
   } else {
      MPI_Gather(local_vec, local_n, MPI_DOUBLE,
        a, local_n, MPI_DOUBLE, 0, comm); //the others will receive the values and store in a, with the amount of local_n values
   }
}


double Perform_dot_product(double local_vec1[], double local_vec2[],
      int local_n, MPI_Comm comm) {
   int local_i;
   double dot_product, local_dot_product = 0;

   for (local_i = 0; local_i < local_n; local_i++)
      local_dot_product += local_vec1[local_i] * local_vec2[local_i];

   MPI_Reduce(&local_dot_product, &dot_product, 1, MPI_DOUBLE, MPI_SUM,
         0, comm);
   return dot_product;
}


void Vec_scalar_mult(double local_vec[], double scalar,
      double local_result[], int local_n) {
   int local_i;
   for (local_i = 0; local_i < local_n; local_i++)
      local_result[local_i] = local_vec[local_i] * scalar;
}
