#include <stdio.h>

#undef NDEBUG

#include <assert.h>
#include <limits.h>
#include <string.h>

// Originally from the Public Domain C Library (PDCLib).

#define testscanf(s, format, ...) sscanf(s, format, __VA_ARGS__)

#define PREPARE_SOURCE(input_string)                                           \
  memcpy(source, input_string, sizeof(input_string));

#define SCANF_TEST(expected_rc, input_string, ...)                             \
  do {                                                                         \
    int actual_rc;                                                             \
    PREPARE_SOURCE(input_string);                                              \
    actual_rc = testscanf(source, __VA_ARGS__);                                \
    assert(actual_rc == expected_rc);                                          \
  } while (0)

int main(void) {
  char source[100];
  char buffer[100];
  int i;
  unsigned int u;
  int *p;
  int n;
  /* basic: reading of three-char string */
  SCANF_TEST(1, "foo", "%3c", buffer);
  assert(memcmp(buffer, "foo", 3) == 0);
  /* %% for single % */
  SCANF_TEST(1, "%x", "%%%c%n", buffer, &n);
  assert(n == 2);
  assert(buffer[0] == 'x');
  /* * to skip assignment */
  SCANF_TEST(0, "abcdefg", "%*[cba]%n", &n);
  assert(n == 3);
  SCANF_TEST(0, "foo", "%*s%n", &n);
  assert(n == 3);
  SCANF_TEST(0, "abc", "%*c%n", &n);
  assert(n == 1);
  SCANF_TEST(1, "3xfoo", "%*dx%3c", buffer);
  assert(memcmp(buffer, "foo", 3) == 0);
  /* domain testing on 'int' type */
  SCANF_TEST(1, "-32768", "%d", &i);
  assert(i == INT_MIN);
  SCANF_TEST(1, "32767", "%d", &i);
  assert(i == INT_MAX);
  SCANF_TEST(1, "-1", "%d", &i);
  assert(i == -1);
  SCANF_TEST(1, "0", "%d", &i);
  assert(i == 0);
  SCANF_TEST(1, "1", "%d", &i);
  assert(i == 1);
  SCANF_TEST(1, "-32768", "%i", &i);
  assert(i == INT_MIN);
  SCANF_TEST(1, "32767", "%i", &i);
  assert(i == INT_MAX);
  SCANF_TEST(1, "-1", "%i", &i);
  assert(i == -1);
  SCANF_TEST(1, "0", "%i", &i);
  assert(i == 0);
  SCANF_TEST(1, "1", "%i", &i);
  assert(i == 1);
  SCANF_TEST(1, "0x7fff", "%i", &i);
  assert(i == INT_MAX);
  SCANF_TEST(1, "0x0", "%i", &i);
  assert(i == 0);
  SCANF_TEST(1, "00", "%i%n", &i, &n);
  assert(i == 0);
  assert(n == 2);
  /* domain testing on 'unsigned int' type */
  SCANF_TEST(1, "65535", "%u", &u);
  assert(u == UINT_MAX);
  SCANF_TEST(1, "0", "%u", &u);
  assert(u == 0);
  SCANF_TEST(1, "ffff", "%x", &u);
  assert(u == UINT_MAX);
  SCANF_TEST(1, "7fff", "%x", &u);
  assert(u == INT_MAX);
  SCANF_TEST(1, "0", "%o", &u);
  assert(u == 0);
  SCANF_TEST(1, "0177777", "%o", &u);
  assert(u == UINT_MAX);
  /* testing %c */
  memset(buffer, '\0', 100);
  SCANF_TEST(1, "x", "%c", buffer);
  assert(memcmp(buffer, "x\0", 2) == 0);
  /* testing %s */
  memset(buffer, '\0', 100);
  SCANF_TEST(1, "foo bar", "%s%n", buffer, &n);
  assert(memcmp(buffer, "foo\0", 4) == 0);
  assert(n == 3);
  SCANF_TEST(2, "foo bar  baz", "%s %s %n", buffer, buffer + 4, &n);
  assert(n == 9);
  assert(memcmp(buffer, "foo\0bar\0", 8) == 0);
  /* testing %[ */
  SCANF_TEST(1, "abcdefg", "%[cba]", buffer);
  assert(memcmp(buffer, "abc\0", 4) == 0);
  SCANF_TEST(-1, "", "%[cba]", buffer);
  SCANF_TEST(1, "3", "%u%[cba]", &u, buffer);
  /* testing %p */
  p = NULL;
  sprintf(buffer, "%p", (void *)p);
  p = &i;
  SCANF_TEST(1, buffer, "%p", (void **)&p);
  assert(p == NULL);
  p = &i;
  sprintf(buffer, "%p", (void *)p);
  p = NULL;
  SCANF_TEST(1, buffer, "%p", (void **)&p);
  assert(p == &i);
  /* errors */
  SCANF_TEST(EOF, "", "%d", &i);
  SCANF_TEST(1, "foo", "%5c", buffer);
  assert(memcmp(buffer, "foo", 3) == 0);
}
