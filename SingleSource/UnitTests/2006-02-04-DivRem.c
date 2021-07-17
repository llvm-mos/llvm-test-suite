
#include <stdio.h>

void test(unsigned long X, unsigned long Y) {
  printf("%lu %lu %lu %lu\n", X, Y, X / (8ul << (Y&15)), X % (8ul << (Y&15)));
}

long i;
int main(void) {
  long long l;

  for(i=10; i<139045193; i*=-3) {
    test(i, i^12345);
    i++;
  }

  return(0);
}

