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

int main(void) {
  char s[] = "abcabcabcdabcde";
  assert(strstr(s, "x") == NULL);
  assert(strstr(s, "xyz") == NULL);
  assert(strstr(s, "a") == &s[0]);
  assert(strstr(s, "abc") == &s[0]);
  assert(strstr(s, "abcd") == &s[6]);
  assert(strstr(s, "abcde") == &s[10]);
  return 0;
}
