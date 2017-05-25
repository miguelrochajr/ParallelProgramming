#include <stdio.h>
#include <stdlib.h>

void Print_two_d(int two[][4], int rows, int cols);

int main(void) {
   int i,j;
   int two_d[3][4];
   int temp = 0;

   for (i = 0; i < 3; i++)
      for(j =0; j<4; j++) {
         two_d[i][j] = temp;
         temp++;
      }

   Print_two_d(two_d, 3, 4);
   return 0;
}  /* main */

void Print_two_d(int two[][4], int rows, int cols) {
   int i,j;
   for (i = 0; i < rows ;i++){
      for(j =0; j < cols;j++) {
         printf("%d ", two[i][j]);
      }
      printf("\n");
   }
}
