#include <float.h>
#include <stdio.h>

int main(void) {
  // Basic
  printf("%f\n", 1.0);
  printf("%f\n", 2.0);
  printf("%f\n", 3.0);
  printf("%f\n", 0.5);

  // 2^53-1; maximum number of 1s.
  printf("%f\n", 9007199254740991.0);

  printf("%f\n", DBL_MAX);
  printf("%.1500f\n", DBL_MIN);

  // Zero
  printf("%f\n", 0.0);

  // Round to even
  printf("%.0f\n", 1.1);
  printf("%.0f\n", 1.9);
  printf("%.0f\n", 1.5);
  printf("%.0f\n", 2.5);
  printf("%.0f\n", 0.9);

  // Alt
  printf("%#.0f\n", 0.0);

  // Negative
  printf("%f\n", -1.0);
  printf("%f\n", -0.0);

  // Infinity
  printf("%f\n", 1.0/0);
  printf("%f\n", -1.0/0);
  printf("%F\n", 1.0/0);
  printf("%4f\n", 1.0/0);
  printf("%04f\n", 1.0/0);
  printf("% f\n", 1.0/0);
  printf("%+f\n", 1.0/0);

  // NaN
  printf("%f\n", 0.0/0);

  // Plus
  printf("%+f\n", 1.0);

  // Space
  printf("% f\n", 1.0);

  // Width
  printf("%1f\n", 0.0);
  printf("%9f\n", 0.0);
  printf("%9f\n", 1.0);
  printf("%10f\n", 10.0);
  printf("%2.f\n", 0.0);
  printf("%#3.f\n", 0.0);
  printf("%+3.f\n", 0.0);
  printf("% 3.f\n", 0.0);

  // Zero
  printf("%09f\n", 0.0);

  // Minus
  printf("%-9fx\n", 0.0);

  // Exp
  printf("%e\n", 0.0);
  printf("%e\n", 1.0);
  printf("%e\n", 2.0);
  printf("%e\n", 0.5);
  printf("%e\n", 10.0);
  printf("%e\n", 1e10);
  printf("%e\n", 1e100);
  printf("%.e\n", 15.0);
  printf("%E\n", 0.0);

  // Exp width
  printf("%6.e\n", 0.0);
  printf("%7.e\n", 1e100);

  // Generic
  printf("%g\n", 0.0);
  printf("%#g\n", 0.0);
  printf("%g\n", 1e-4);
  printf("%g\n", 1e-5);
  printf("%g\n", 1e5);
  printf("%g\n", 1e6);
  printf("%g\n", 0.0001234567);
  printf("%g\n", 0.00001234567);

  // Hex
  printf("%a\n", 0.0);
  printf("%A\n", 0.0);
  printf("%#a\n", 0.0);
  printf("%a\n", 1.0);
  printf("%a\n", 0x1p128);
  printf("%.6a\n", 0.0);
  printf("%a\n", 0.1234567);
  printf("%6a\n", 0.0);
  printf("%7a\n", 0.0);
  printf("%7a\n", 0x1p10);
  printf("%8a\n", 0x1p10);
}
