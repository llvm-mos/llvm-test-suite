// Copyright 2024 LLVM-MOS
//
// Licensed under the Apache License, Version 2.0 with LLVM Exceptions,
// See https://github.com/llvm-mos/llvm-mos-sdk/blob/main/LICENSE for license
// information.

#undef NDEBUG
#include <assert.h>
#include <stdlib.h>
#include <string.h>

// Originally from the Public Domain C Library (PDCLib)

static const char abcde[] = "abcde";

static int compare(const void *left, const void *right) {
  return *((unsigned char *)left) - *((unsigned char *)right);
}

int main(void) {
  assert(bsearch("e", abcde, 4, 1, compare) == NULL);
  assert(bsearch("e", abcde, 5, 1, compare) == &abcde[4]);
  assert(bsearch("a", abcde + 1, 4, 1, compare) == NULL);
  assert(bsearch("0", abcde, 1, 1, compare) == NULL);
  assert(bsearch("a", abcde, 1, 1, compare) == &abcde[0]);
  assert(bsearch("a", abcde, 0, 1, compare) == NULL);
  assert(bsearch("e", abcde, 3, 2, compare) == &abcde[4]);
  assert(bsearch("b", abcde, 3, 2, compare) == NULL);
  return 0;
}
