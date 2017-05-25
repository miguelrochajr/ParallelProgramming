
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

int my_rank, comm_sz;
MPI_Comm comm;


void Get_n(int* n_p);
void Read_loc_mat(double loc_mat[], int n);
void Build_indexed_type(int n, MPI_Datatype* indexed_mpi_t_p);
void Print_loc_mat(char title[], double loc_mat[], int n);


int main(void) {
   int n;
   double *loc_mat;
   MPI_Datatype indexed_mpi_t;

   MPI_Init(NULL, NULL);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &comm_sz);
   MPI_Comm_rank(comm, &my_rank);

   if (comm_sz < 2){
     printf("Please provide n greater than 2 \n", );
     return -1;
   }

   Get_n(&n);
   loc_mat = malloc(n*n*sizeof(double));

   Build_indexed_type(n, &indexed_mpi_t);
   if (my_rank == 0) {
      Read_loc_mat(loc_mat, n);
      MPI_Send(loc_mat, 1, indexed_mpi_t, 1, 0, comm);
   } else if (my_rank ==1){
      memset(loc_mat, 0, n*n*sizeof(double)); // for the local_matix, fill it with zeros
      MPI_Recv(loc_mat, 1, indexed_mpi_t, 0, 0, comm, MPI_STATUS_IGNORE); //receives the resulting triangular matrix of type indexed_mpi_t
      Print_loc_mat("Received matrix", loc_mat, n); //prints it
   }

   free(loc_mat);
   MPI_Type_free(&indexed_mpi_t);
   MPI_Finalize();
   return 0;
}

void Get_n(int*    n_p          /* out */) {

   if (my_rank == 0) {
      printf("Enter the order of the matrix\n");
      scanf("%d", n_p);
   }

   MPI_Bcast(n_p, 1, MPI_INT, 0, comm);

}

void Build_indexed_type(
      int            n                /* in  */,
      MPI_Datatype*  indexed_mpi_t_p  /* out */) {
   int i;
   int* array_of_block_lens;
   int* array_of_disps;

   array_of_block_lens = malloc(n*sizeof(int));
   array_of_disps = malloc(n*sizeof(int));

   for (i = 0; i < n ; i++) {
      array_of_block_lens[i] = n-i; /* Note here that for a upper triangular, the blocks will be decrezing size */
      array_of_disps[i] = i*(n+1);  /* For the array of displacements, we must remind the way it is saved into memory,
                                      as a big continuous array that a[i][j] = i*(cols+1) */
   }

   MPI_Type_indexed(n,            /* square matrix order. Here, the # of blocks */
          array_of_block_lens,    /* # of elements per block */
          array_of_disps,         /* displacement for each block, in multiples of oldtype extend for MPI_Type_indexed */
                                  /* and bytes for MPI_type_create */
          MPI_DOUBLE,             /* Old mpi type */
          indexed_mpi_t_p);       /* New datatype */
   MPI_Type_commit(indexed_mpi_t_p); /* DO NOT FORGET TO COMMIT YOUR CHANGES! */

   free(array_of_block_lens);
   free(array_of_disps);
}

void Read_loc_mat(
             double    loc_mat[]  /* out */,
             int       n          /* in  */) {
   int i,j;

   printf("Enter the matrix\n");
   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
         scanf("%lf", &loc_mat[i*n + j]); // Againg, the continuous array is used here to "simulate" a matrix.
}


void Print_loc_mat(
             char      title[]    /* in */,
             double    loc_mat[]  /* in */,
             int       n          /* in */) {
   int i,j;

   printf("Proc %d > %s\n", my_rank, title);
   for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++)
         printf("%.2f ", loc_mat[i*n + j]);
      printf("\n");
   }
   printf("\n");
}
