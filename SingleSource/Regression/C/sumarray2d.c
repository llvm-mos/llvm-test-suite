
#include <stdio.h>
#include <stdlib.h>

long SumArray(int Array[][100], unsigned int NumI, unsigned int NumJ) {
  unsigned i, j;
  long Result = 0;
  
  for (i = 0; i < NumI; i++)
    for (j = 0; j < NumJ; j++)
      Result += Array[i][j];
  
  return Result;
}

int main() {
  int Array[100][100];
  unsigned int i, j;
  
  for (i = 0; i < 100; i++)
    Array[i][i] = -i;
  
  for (i = 0; i < 100; i++)
    for (j = 0; j < 100; j++)
      if (j != i)
        Array[i][j] = i+j;
  
  printf("Sum(Array[%d,%d] = %ld\n", 100, 100, SumArray(Array, 100, 100));
  
  return 0;
}
