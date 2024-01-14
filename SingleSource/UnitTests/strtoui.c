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
  assert(strtoui("123", NULL, 10) == 123);
  /* proper detecting of default base 10 */
  assert(strtoui("456", NULL, 0) == 456);
  /* proper functioning to smaller base */
  assert(strtoui("14", NULL, 8) == 12);
  /* proper autodetecting of octal */
  assert(strtoui("016", NULL, 0) == 14);
  /* proper autodetecting of hexadecimal, lowercase 'x' */
  assert(strtoui("0xFF", NULL, 0) == 255);
  /* proper autodetecting of hexadecimal, uppercase 'X' */
  assert(strtoui("0Xa1", NULL, 0) == 161);
  /* proper handling of border case: 0x followed by non-hexdigit */
  assert(strtoui(tricky, &endptr, 0) == 0);
  assert(endptr == tricky + 2);
  /* proper handling of border case: 0 followed by non-octdigit */
  assert(strtoui(tricky, &endptr, 8) == 0);
  assert(endptr == tricky + 2);
  /* errno should still be 0 */
  assert(errno == 0);
  /* correctly decoding zero */
  assert(strtoui("0", &endptr, 0) == 0);
  assert(*endptr == '\0');
  assert(errno == 0);
  /* overflowing subject sequence must still return proper endptr */
  assert(strtoui(overflow, &endptr, 36) == UINT_MAX);
  assert(errno == ERANGE);
  assert((endptr - overflow) == 53);
  /* same for positive */
  errno = 0;
  assert(strtoui(overflow + 1, &endptr, 36) == UINT_MAX);
  assert(errno == ERANGE);
  assert((endptr - overflow) == 53);
  /* testing skipping of leading whitespace */
  assert(strtoui(" \n\v\t\f789", NULL, 0) == 789);
  /* testing conversion failure */
  assert(strtoui(overflow, &endptr, 10) == 0);
  assert(endptr == overflow);
  endptr = NULL;
  assert(strtoui(overflow, &endptr, 0) == 0);
  assert(endptr == overflow);
  /* TODO: These tests assume two-complement, but conversion should work */
  /* for one-complement and signed magnitude just as well. Anyone having */
  /* a platform to test this on?                                         */
  errno = 0;
  /* long -> 16 bit */
  /* testing "even" overflow, i.e. base is power of two */
  assert(strtoui("65535", NULL, 0) == UINT_MAX);
  assert(errno == 0);
  errno = 0;
  assert(strtoui("65536", NULL, 0) == UINT_MAX);
  assert(errno == ERANGE);
  return 0;
}
