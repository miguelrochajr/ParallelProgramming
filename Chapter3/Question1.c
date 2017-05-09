/*
- Chapter 3

- Question 1

What happens in the greetins program if, instead of strlen(greeetin)+1, we use strlen(greeting) for the length of the message being sent by processes 1,2,...,comm_sz-1?
What happens if we use MAX_STRING instead of strlen(greeting)+1? Can you explain these results?


The strlen(greeting)+1 is the number of characters in the message plus one characther for the '\0' char. Which, in C, represents the end of a string. If we use only
strlen(greeting) we would be sending the String message without the '\0' charachter. This could lead to plenty of problems. The most dangerous of them is segmentation fault.
Since we are seding the string "without an end", an implementation that keeps parsing the string up to an '\0' char would never stop and probably attempt to access a memory
location that does not exist.

In the case we use MAX_STRING, we have some small differences. Despite our output being the same, the MPI_Send will send all the 100 locations to the receiver. In the case
of our greeting string, all of it will be sent and not only the actual text. Since we are also sending the \0, the output will be the same since printf looks for it to end
the string output.
*/

#include <stdio.h>
#include <string.h>  /* For strlen             */
#include <mpi.h>     /* For MPI functions, etc */

const int MAX_STRING = 100;

int main(void) {
   char       greeting[MAX_STRING];  /* String storing message */
   int        comm_sz;               /* Number of processes    */
   int        my_rank;               /* My process rank        */

   /* Start up MPI */
   MPI_Init(NULL, NULL);

   /* Get the number of processes */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

   /* Get my rank among all the processes */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

   if (my_rank != 0) {
      /* Create message */
      sprintf(greeting, "Greetings from process %d of %d!",
            my_rank, comm_sz);
      /* Send message to process 0 */
      MPI_Send(greeting, MAX_STRING, MPI_CHAR, 0, 0, MPI_COMM_WORLD); //
   } else {
      /* Print my message */
      printf("Greetings from process %d of %d!\n", my_rank, comm_sz);
      for (int q = 1; q < comm_sz; q++) {
         /* Receive message from process q */
         MPI_Recv(greeting, MAX_STRING, MPI_CHAR, q,
            0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         /* Print message from process q */
         printf("%s < message. And the strlen: %lu \n", greeting, strlen(greeting));
      }
   }

   /* Shut down MPI */
   MPI_Finalize();

   return 0;
}
