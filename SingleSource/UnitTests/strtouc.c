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
  assert(_strtouc("123", NULL, 10) == 123);
  /* proper functioning to smaller base */
  assert(_strtouc("14", NULL, 8) == 12);
  /* proper autodetecting of octal */
  assert(_strtouc("016", NULL, 0) == 14);
  /* proper autodetecting of hexadecimal, lowercase 'x' */
  assert(_strtouc("0xFF", NULL, 0) == 255);
  /* proper autodetecting of hexadecimal, uppercase 'X' */
  assert(_strtouc("0Xa1", NULL, 0) == 161);
  /* proper handling of border case: 0x followed by non-hexdigit */
  assert(_strtouc(tricky, &endptr, 0) == 0);
  assert(endptr == tricky + 2);
  /* proper handling of border case: 0 followed by non-octdigit */
  assert(_strtouc(tricky, &endptr, 8) == 0);
  assert(endptr == tricky + 2);
  /* errno should still be 0 */
  assert(errno == 0);
  /* correctly decoding zero */
  assert(_strtouc("0", &endptr, 0) == 0);
  assert(*endptr == '\0');
  assert(errno == 0);
  /* overflowing subject sequence must still return proper endptr */
  assert(_strtouc(overflow, &endptr, 36) == UCHAR_MAX);
  assert(errno == ERANGE);
  assert((endptr - overflow) == 53);
  /* same for positive */
  errno = 0;
  assert(_strtouc(overflow + 1, &endptr, 36) == UCHAR_MAX);
  assert(errno == ERANGE);
  assert((endptr - overflow) == 53);
  /* testing skipping of leading whitespace */
  assert(_strtouc(" \n\v\t\f123", NULL, 0) == 123);
  /* testing conversion failure */
  assert(_strtouc(overflow, &endptr, 10) == 0);
  assert(endptr == overflow);
  endptr = NULL;
  assert(_strtouc(overflow, &endptr, 0) == 0);
  assert(endptr == overflow);
  /* TODO: These tests assume two-complement, but conversion should work */
  /* for one-complement and signed magnitude just as well. Anyone having */
  /* a platform to test this on?                                         */
  errno = 0;
  /* long -> 32 bit */
  /* testing "even" overflow, i.e. base is power of two */
  assert(_strtouc("255", NULL, 0) == UCHAR_MAX);
  assert(errno == 0);
  errno = 0;
  assert(_strtouc("256", NULL, 0) == UCHAR_MAX);
  assert(errno == ERANGE);
  /* TODO: test "odd" overflow, i.e. base is not power of two */
  /* long -> 64 bit */
  return 0;
}
