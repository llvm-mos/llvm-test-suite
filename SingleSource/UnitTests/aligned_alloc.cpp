#undef NDEBUG
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

int main() {
  void* align_8 = aligned_alloc(8, 1);
  assert(align_8);
  assert((uintptr_t)align_8 % 8 == 0);

  void* align_16 = aligned_alloc(16, 1);
  assert(align_16);
  assert((uintptr_t)align_16 % 16 == 0);

  free(align_8);
  void* align_2 = aligned_alloc(2, 1);
  assert(align_2);
  assert((uintptr_t)align_8 % 2 == 0);

  void* align_1 = aligned_alloc(1, 42);
  assert(align_1);

  // Alignment must be a power of two
  void* align_3 = aligned_alloc(3, 1);
  assert(!align_3);
}
