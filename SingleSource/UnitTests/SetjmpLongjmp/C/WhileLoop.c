#include <setjmp.h>
#include <stdio.h>

void foo(jmp_buf buf, int i)
{
  printf("Inside foo: %d\n", i);
  longjmp(buf, i);
}

int main()
{
  int i = 37;

  while (i--) {
    jmp_buf buf;
    if (setjmp(buf)) {
      // i is unchanged between setjmp and longjmp, including the zero argument.
      printf("Return from longjmp: %d\n", i ? i : 1);
    } else {
      foo(buf, i);
    }
  }

  return 0;
}
