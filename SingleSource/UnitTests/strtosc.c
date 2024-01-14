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
  assert(strtosc("123", NULL, 10) == 123);
  /* proper functioning to smaller base */
  assert(strtosc("14", NULL, 8) == 12);
  /* proper autodetecting of octal */
  assert(strtosc("016", NULL, 0) == 14);
  /* proper handling of border case: 0x followed by non-hexdigit */
  assert(strtosc(tricky, &endptr, 0) == 0);
  assert(endptr == tricky + 2);
  /* proper handling of border case: 0 followed by non-octdigit */
  assert(strtosc(tricky, &endptr, 8) == 0);
  assert(endptr == tricky + 2);
  /* errno should still be 0 */
  assert(errno == 0);
  /* correctly decoding zero */
  assert(strtosc("0", &endptr, 0) == 0);
  assert(*endptr == '\0');
  assert(errno == 0);
  /* overflowing subject sequence must still return proper endptr */
  assert(strtosc(overflow, &endptr, 36) == SCHAR_MIN);
  assert(errno == ERANGE);
  assert((endptr - overflow) == 53);
  /* same for positive */
  errno = 0;
  assert(strtosc(overflow + 1, &endptr, 36) == SCHAR_MAX);
  assert(errno == ERANGE);
  assert((endptr - overflow) == 53);
  /* testing conversion failure */
  assert(strtosc(overflow, &endptr, 10) == 0);
  assert(endptr == overflow);
  endptr = NULL;
  assert(strtosc(overflow, &endptr, 0) == 0);
  assert(endptr == overflow);
  /* TODO: These tests assume two-complement, but conversion should work */
  /* for one-complement and signed magnitude just as well. Anyone having */
  /* a platform to test this on?                                         */
  errno = 0;
  /* testing "even" overflow, i.e. base is power of two */
  assert(strtosc("127", NULL, 0) == 0x7f);
  assert(errno == 0);
  errno = 0;
  assert(strtosc("128", NULL, 0) == SCHAR_MAX);
  assert(errno == ERANGE);
  errno = 0;
  assert(strtosc("-127", NULL, 0) == (signed char)0x81);
  assert(errno == 0);
  errno = 0;
  assert(strtosc("-128", NULL, 0) == SCHAR_MIN);
  assert(errno == 0);
  errno = 0;
  assert(strtosc("-129", NULL, 0) == SCHAR_MIN);
  assert(errno == ERANGE);
  return 0;
}
