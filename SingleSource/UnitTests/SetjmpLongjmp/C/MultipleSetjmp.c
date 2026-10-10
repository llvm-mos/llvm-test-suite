#include <setjmp.h>
#include <stdio.h>

int bar(jmp_buf buf, int i)
{
  printf("Inside bar %d\n", i);
  longjmp(buf, i);
}

int main()
{
  jmp_buf buf;
  switch (setjmp(buf)) {
  case 37:
    printf("returning from bar == 37\n");

    switch (setjmp(buf)) {
    case 927:
      printf("returning from bar == 927\n");
      break;
    case 0:
      bar(buf, 927);
      break;
    default:
      printf("Unexpected inner setjmp return value\n");
      return 1;
    }
    break;
  case 0:
    bar(buf, 37);
    break;
  default:
    printf("Unexpected outer setjmp return value\n");
    return 1;
  }

  return 0;
}
