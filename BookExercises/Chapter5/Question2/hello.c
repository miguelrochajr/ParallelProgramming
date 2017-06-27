#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char const *argv[]) {

  int thread_count = strtol(argv[1], NULL, 10);
  printf("Hello World from %d\n", thread_count);
  return 0;
}
