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
static const char abcdx[] = "abcdx";

int main(void) {
  assert(strcspn(abcde, "x") == 5);
  assert(strcspn(abcde, "xyz") == 5);
  assert(strcspn(abcde, "zyx") == 5);
  assert(strcspn(abcdx, "x") == 4);
  assert(strcspn(abcdx, "xyz") == 4);
  assert(strcspn(abcdx, "zyx") == 4);
  assert(strcspn(abcde, "a") == 0);
  assert(strcspn(abcde, "abc") == 0);
  assert(strcspn(abcde, "cba") == 0);
  return 0;
}
