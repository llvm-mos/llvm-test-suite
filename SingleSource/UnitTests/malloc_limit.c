#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

#ifdef __LLVM_MOS_SDK
  __set_heap_limit(4096);
#endif

  static void *an_alloc = NULL;
  an_alloc = malloc(500);

  if (an_alloc != NULL) {
    free(an_alloc);
  } else {
    abort();
  }

#ifdef __LLVM_MOS_SDK
  an_alloc = malloc(40000); // Must return NULL.
  if (an_alloc) {
    abort();
  }
#endif

#define ALLOC_COUNT 8

  static const size_t alloc_sizes[ALLOC_COUNT] = {1, 4, 16, 7, 35, 100, 3, 9};
  static uint8_t *allocations[ALLOC_COUNT] = {};

  for (uint8_t i = 0; i < ALLOC_COUNT; i += 1) {
    const uint8_t alloc_sz = alloc_sizes[i];
    uint8_t *const new_alloc = (uint8_t *)malloc(alloc_sz);
    if (new_alloc == NULL) {
      abort();
    }

    memset(new_alloc, (int)alloc_sz, alloc_sz);
    allocations[i] = new_alloc;
  }

  // dealloc in a different order than alloc
  static uint8_t dealloc_order[ALLOC_COUNT] = {4, 6, 2, 0, 3, 1, 7, 5};

  for (uint8_t i = 0; i < ALLOC_COUNT; i += 1) {
    const uint8_t test_alloc_idx = dealloc_order[i];
    uint8_t *const alloc_to_test = allocations[test_alloc_idx];
    const uint8_t test_alloc_size = alloc_sizes[test_alloc_idx];

    for (uint8_t j = 0; j < test_alloc_size; j += 1) {
      if (alloc_to_test[j] != test_alloc_size) {
        abort();
      }
    }

    free(alloc_to_test);
  }

  return 0;
}
