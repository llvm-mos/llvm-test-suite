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


static int compare(const void *left, const void *right) {
  return *((unsigned char *)left) - *((unsigned char *)right);
}

int main(void) {
  char presort[] = {"shreicnyjqpvozxmbt"};
  char sorted1[] = {"bcehijmnopqrstvxyz"};
  char sorted2[] = {"bticjqnyozpvreshxm"};
  char s[19];
  strcpy(s, presort);
  qsort(s, 18, 1, compare);
  assert(strcmp(s, sorted1) == 0);
  strcpy(s, presort);
  qsort(s, 9, 2, compare);
  assert(strcmp(s, sorted2) == 0);
  strcpy(s, presort);
  qsort(s, 1, 1, compare);
  assert(strcmp(s, presort) == 0);
  qsort(s, 100, 0, compare);
  assert(strcmp(s, presort) == 0);
  return 0;
}
