// Modified by Daniel Thornburgh (mysterymath@gmail.com).

#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS 1
#endif
#include <inttypes.h>

extern int printf(const char *, ...);

void testL(int64_t Arg) {
  printf("%ld\n", (long) (Arg / ((int64_t)1 << 4)));
  printf("%ld\n", (long) (Arg / ((int64_t)1 << 46)));
}

void test(long Arg) {
  printf("%ld\n", Arg / (1L << 0));
  printf("%ld\n", Arg / (1L << 4));
  printf("%ld\n", Arg / (1L << 18));
  printf("%ld\n", Arg / (1L << 30));
}

int main() {
  long B20 = - (1L << 20);
  int64_t B53 = - ((int64_t)1 << 53);

  test(B20 + 32);
  test(B20 + 33);

  testL(B53 + 64);
  testL(B53 + 65);

  return 0;
}
