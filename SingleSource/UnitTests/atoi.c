#undef NDEBUG
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

// Originally from the Public Domain C Library (PDCLib).
#undef NDEBUG
#include <assert.h>
#include <stdlib.h>

// Originally from the Public Domain C Library (PDCLib).

int main(void) {
  /* basic functionality */
  assert(atoi("123") == 123);
  /* testing skipping of leading whitespace and trailing garbage */
  assert(atoi(" \n\v\t\f123xyz") == 123);
  // Negative
  assert(atoi("-123") == -123);
  return 0;
}
