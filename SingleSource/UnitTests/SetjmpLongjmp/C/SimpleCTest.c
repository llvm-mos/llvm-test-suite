#include <setjmp.h>
#include <stdio.h>

void baz(jmp_buf buf)
{
  printf("Inside baz\n");
  longjmp(buf, 37);
}

int main()
{
  jmp_buf buf;

  printf("Inside main\n");

  switch (setjmp(buf)) {
  case 0:
    baz(buf);
    break;
  case 37:
    printf("ret == 37\n");
    break;
  default:
    printf("Unexpected setjmp return value\n");
    return 1;
  }

  return 0;
}
