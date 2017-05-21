#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(void) {
   //int n, local_n;
   int comm_sz, my_rank;
   //double *local_x, *local_y, *local_z;
   MPI_Init(NULL, NULL);
   //comm = MPI_COMM_WORLD;

   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

   int tamanho = 4;
   int i,j;
   //int **matrix = malloc(tamanho* sizeof(int*));

    if(my_rank == 0){
    printf("preencha a matrix \n");
    }


   int **matrix = (int**)malloc(tamanho*sizeof(int*));
   if(my_rank == 0){
   printf("preencha a matrix \n");
   }

   if(my_rank == 0){
     printf("preencha a matrix \n");

     for (i=0;i<tamanho;i++){
      matrix[i] = malloc(tamanho*sizeof(int));
      //matrix[i] = (int*)malloc(tamanho*sizeof(int));
     }
     for(i=0;i<tamanho;i++){
      for(j=0;j<tamanho;j++){
        scanf("%d",&matrix[i][j]);
      }
     }
   }




  //  int* array_of_blocklengths = (int*) malloc(sizeof(int) * tamanho);
  //  int* array_of_displacements = (int*) malloc(sizeof(int) * tamanho);

  if(my_rank == 0){
    printf("diga agora \n");
   }

   int *array_of_blocklengths = malloc(tamanho*sizeof(int));
   int *array_of_displacements= malloc(tamanho*sizeof(int));

   if(my_rank == 0){
     printf("diga agora 1 \n");
    }
   for (i=0;i<tamanho;i++){
	   array_of_displacements[i] = i;
	   array_of_blocklengths[i] = tamanho - i;
   }

   if(my_rank == 0){
     printf("diga agora 2\n");
    }

   MPI_Datatype tipo_novo;
   MPI_Type_indexed(tamanho, array_of_blocklengths, array_of_displacements, MPI_INT, &tipo_novo);
   MPI_Type_commit(&tipo_novo);
   if(my_rank == 0){
     printf("diga agora 3 \n");
    }

   if(my_rank == 0){
	   MPI_Send(matrix, 1, tipo_novo, 1, 0, MPI_COMM_WORLD);
   }
   else{
	   int count = 10;
	   int *matrix_recv = (int*) malloc(sizeof(int) *tamanho*tamanho);
	   MPI_Recv(matrix_recv, tamanho*tamanho, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	   //MPI_Get_count(MPI_STATUS_IGNORE, MPI_INTEGER, &count );

	   for (i=0;i<count;i++){
		   printf("O elemento %d da diagonal superior eh, %d \n",i,matrix_recv[i]);
	   }
   }
   MPI_Finalize();

}
