#undef NDEBUG
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

// Originally from the Public Domain C Library (PDCLib).

int main(void) {
  char *endptr;
  /* this, to base 36, overflows even a 256 bit integer */
  char overflow[] = "-ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ_";
  /* tricky border case */
  char tricky[] = "+0xz";
  errno = 0;
  /* basic functionality */
  assert(strtoi("123", NULL, 10) == 123);
  /* proper detecting of default base 10 */
  assert(strtoi("456", NULL, 0) == 456);
  /* proper functioning to smaller base */
  assert(strtoi("14", NULL, 8) == 12);
  /* proper autodetecting of octal */
  assert(strtoi("016", NULL, 0) == 14);
  /* proper autodetecting of hexadecimal, lowercase 'x' */
  assert(strtoi("0xFF", NULL, 0) == 255);
  /* proper autodetecting of hexadecimal, uppercase 'X' */
  assert(strtoi("0Xa1", NULL, 0) == 161);
  /* proper handling of border case: 0x followed by non-hexdigit */
  assert(strtoi(tricky, &endptr, 0) == 0);
  assert(endptr == tricky + 2);
  /* proper handling of border case: 0 followed by non-octdigit */
  assert(strtoi(tricky, &endptr, 8) == 0);
  assert(endptr == tricky + 2);
  /* errno should still be 0 */
  assert(errno == 0);
  /* correctly decoding zero */
  assert(strtoi("0", &endptr, 0) == 0);
  assert(*endptr == '\0');
  assert(errno == 0);
  /* overflowing subject sequence must still return proper endptr */
  assert(strtoi(overflow, &endptr, 36) == INT_MIN);
  assert(errno == ERANGE);
  assert((endptr - overflow) == 53);
  /* same for positive */
  errno = 0;
  assert(strtoi(overflow + 1, &endptr, 36) == INT_MAX);
  assert(errno == ERANGE);
  assert((endptr - overflow) == 53);
  /* testing skipping of leading whitespace */
  assert(strtoi(" \n\v\t\f789", NULL, 0) == 789);
  /* testing conversion failure */
  assert(strtoi(overflow, &endptr, 10) == 0);
  assert(endptr == overflow);
  endptr = NULL;
  assert(strtoi(overflow, &endptr, 0) == 0);
  assert(endptr == overflow);
  /* TODO: These tests assume two-complement, but conversion should work */
  /* for one-complement and signed magnitude just as well. Anyone having */
  /* a platform to test this on?                                         */
  errno = 0;
  /* testing "even" overflow, i.e. base is power of two */
  assert(strtoi("32767", NULL, 0) == 0x7fff);
  assert(errno == 0);
  errno = 0;
  assert(strtoi("32768", NULL, 0) == INT_MAX);
  assert(errno == ERANGE);
  errno = 0;
  assert(strtoi("-32767", NULL, 0) == (int)0x8001);
  assert(errno == 0);
  errno = 0;
  assert(strtoi("-32768", NULL, 0) == INT_MIN);
  assert(errno == 0);
  errno = 0;
  assert(strtoi("-32769", NULL, 0) == INT_MIN);
  assert(errno == ERANGE);
  return 0;
}
