#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int my_rank, comm_sz;
MPI_Comm comm;

void Read_n(int* n_p, int* local_n_p);
void Init_counts_displs(int counts[], int displs[], int n);
void Read_vect(double local_a[], int local_n, int n, char vec_name[]);
void Print_vect(double local_b[], int local_n, int n, char title[]);
void Par_vect_sum(double local_x[], double local_y[],
                   double local_z[], int local_n);
double Par_dot_prod(double local_vec1[], double local_vec2[], int local_n);

int main(void) {
   int n, local_n;
   double *local_x, *local_y, *local_z;
   double dot_product;

   MPI_Init(NULL, NULL);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &comm_sz);
   MPI_Comm_rank(comm, &my_rank);

   Read_n(&n, &local_n);

//   Allocate_vects(&local_x, &local_y, &local_z, local_n);
   local_x = malloc(local_n*sizeof(double));
   local_y = malloc(local_n*sizeof(double));
   local_z = malloc(local_n*sizeof(double));


   Read_vect(local_x, local_n, n, "x");
   Print_vect(local_x, local_n, n, "x is");
   Read_vect(local_y, local_n, n, "y");
   Print_vect(local_y, local_n, n, "y is");

   Par_vect_sum(local_x, local_y, local_z, local_n);
   Print_vect(local_z, local_n, n, "The sum is");

   dot_product = Par_dot_prod(local_x, local_y, local_n);
   if (my_rank == 0)
      printf("Dot product input vectors is %f\n", dot_product);

   free(local_x);
   free(local_y);
   free(local_z);

   MPI_Finalize();
   return 0;
}

void Read_n(
         int*      n_p        /* out */,
         int*      local_n_p  /* out */) {
   int quotient, remainder;

   if (my_rank == 0) {
      printf("What's the order of the vectors?\n");
      scanf("%d", n_p);
   }
   MPI_Bcast(n_p, 1, MPI_INT, 0, comm);

   quotient = *n_p/comm_sz;
   remainder = *n_p % comm_sz;

   if (my_rank < remainder) {
      *local_n_p = quotient +1;
   } else {
      *local_n_p = quotient;
   }
}

void Init_counts_displs(int counts[], int displs[], int n) {
   int offset, q, quotient, remainder;
   // Remmember from man page:
   //  displs:    Integer array (of length group size). Entry i specifies the displacement relative to  recvbuf
   //           at which to place the incoming data from process i (significant only at root).
   // counts:     Integer array (of length group size) containing the number of elements that are received from
   //           each process (significant only at root).

   quotient = n/comm_sz;
   remainder = n % comm_sz;
   offset = 0;
   for (q = 0; q < comm_sz; q++) {
      if (q < remainder)
         counts[q] = quotient+1;
      else
         counts[q] = quotient;
      displs[q] = offset;
      offset += counts[q];
   }
}

void Read_vect(
             double    local_a[]   /* out */,
             int       local_n     /* in  */,
             int       n           /* in  */,
             char      vec_name[]  /* in  */) {

   int* counts = NULL;
   int* displs = NULL;

   double* a = NULL; //auxiliar vector to receive from the user.
   int i;

   if (my_rank == 0) {
      counts = malloc(comm_sz*sizeof(int));
      displs = malloc(comm_sz*sizeof(int));

      Init_counts_displs(counts, displs, n);

      a = malloc(n*sizeof(double)); //auxiliar vector

      printf("Enter the vector %s\n", vec_name);
      for (i = 0; i < n; i++)
         scanf("%lf", &a[i]); //receives vector from user and put into auxiliar vector
      MPI_Scatterv(a,     //sending auxiliar buffer
              counts,     //vector with # of elements in each process. Process_rank=index
              displs,     //displacement vector. The process my_rank has displs[my_rank] of dispacement
              MPI_DOUBLE, //The values are of type MPI_DOUBLE
              local_a,    //The receiving buffer
              local_n,    //Address of the receiving buffer. Here will be local_x or local_y
              MPI_DOUBLE, //type of the receiving buffer
              0,          //source process
              comm);      // communicator
      free(a);
      free(displs);
      free(counts);
   } else {
      MPI_Scatterv(a, counts, displs, MPI_DOUBLE,
            local_a, local_n, MPI_DOUBLE, 0, comm);
   }
}

void Print_vect(
              double    local_b[]  /* in */,
              int       local_n    /* in */,
              int       n          /* in */,
              char      title[]    /* in */) {

   int* counts = NULL;
   int* displs = NULL;

   double* b = NULL;
   double* aux = NULL;
   int i;

   if (my_rank == 0) {
      counts = malloc(comm_sz*sizeof(int));
      displs = malloc(comm_sz*sizeof(int));
      Init_counts_displs(counts, displs, n);

      b = malloc(n*sizeof(double));

      MPI_Gatherv(local_b,  // Starting address of sednding buffer
            local_n,        // Number of elements in send buffer
            MPI_DOUBLE,     // Type of sending buffer
            b,              // receiving buffer. This will have all vector elements
            counts,         // receving counts array.
            displs,         // the displacement vector for each process
            MPI_DOUBLE,     // type fo receiving value
            0,              // source process
            comm);          // communicator
      printf("%s\n", title);
      for (i = 0; i < n; i++)
         printf("%f ", b[i]);
      printf("\n");
      free(displs);
      free(counts);
      free(b);
   } else {
      MPI_Gatherv(local_b, local_n, MPI_DOUBLE, b, counts, displs,
            MPI_DOUBLE, 0, comm);
   }
}

void Par_vect_sum(
                   double  local_x[]  /* in  */,
                   double  local_y[]  /* in  */,
                   double  local_z[]  /* out */,
                   int     local_n    /* in  */) {
   int local_i;

   for (local_i = 0; local_i < local_n; local_i++)
      local_z[local_i] = local_x[local_i] + local_y[local_i];
}

double Par_dot_prod(double local_vec1[], double local_vec2[],
      int local_n) {

   int local_i;
   double loc_dot_prod = 0, dot_prod;

   for (local_i = 0; local_i < local_n; local_i++)
      loc_dot_prod += local_vec1[local_i] * local_vec2[local_i];

   MPI_Reduce(&loc_dot_prod, &dot_prod, 1, MPI_DOUBLE, MPI_SUM, 0, comm);

   return dot_prod;
}
