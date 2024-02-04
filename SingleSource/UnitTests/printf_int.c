#include <stdio.h>

int main(void) {
  // Basic
  printf("%d\n", 0);
  printf("%d\n", 6502);

  // Precision
  printf("%.d\n", 0);
  printf("%.0d\n", 0);
  printf("%.1d\n", 0);
  printf("%.2d\n", 0);
  printf("%.*d\n", -5, 0);
  printf("%.3d\n", 42);
  printf("%.2d\n", 42);
  printf("%.1d\n", 42);
  printf("%.d\n", 42);

  // Minus
  printf("%.3d\n", -42);

  // Plus
  printf("%+.3d\n", 42);
  printf("%+.3d\n", -42);
  printf("%+.3d\n", -42);

  // Space
  printf("% d\n", 0);
  printf("% d\n", -1);
  printf("% +d\n", 1);

  // Alternate
  printf("%#o\n", 042);
  printf("%#.o\n", 042);
  printf("%#x\n", 0xab);
  printf("%#X\n", 0xab);

  // Padding
  printf("%2dx\n", 0);
  printf("%-2dx\n", 0);
  printf("%2dx\n", 123);
  printf("%3dx\n", -1);
  printf("%+3dx\n", 1);
  printf("% 3dx\n", 1);
  printf("%#4xx\n", 1);

  // Zero
  printf("%02dx\n", 0);
  printf("%-02dx\n", 0);
}
