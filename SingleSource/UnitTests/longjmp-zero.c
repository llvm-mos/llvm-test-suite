#include <setjmp.h>
#include <stdio.h>

// longjmp(env, val) makes setjmp return val, except that val == 0 makes it
// return 1. The values cover zero, small, a value needing the high byte of a
// 16-bit int, and all bits set.

static jmp_buf env;

// Returns what setjmp returns on the non-local return. The flag is volatile
// because it changes between setjmp and longjmp.
static int jump(int val) {
  volatile int jumped = 0;
  int ret = setjmp(env);
  if (!jumped) {
    jumped = 1;
    longjmp(env, val);
  }
  return ret;
}

int main(void) {
  static const int values[] = {0, 1, 7, 256, -1};
  for (unsigned i = 0; i < sizeof values / sizeof values[0]; i++) {
    int val = values[i];
    int expected = val ? val : 1;
    int got = jump(val);
    if (got != expected) {
      printf("longjmp(env, %d): setjmp returned %d, expected %d\n", val, got,
             expected);
      return 1;
    }
  }
  return 0;
}
