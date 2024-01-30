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

int main(void) {
  assert(strspn(abcde, "abc") == 3);
  assert(strspn(abcde, "b") == 0);
  assert(strspn(abcde, abcde) == 5);
  return 0;
}
