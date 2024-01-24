#include <stdlib.h>
#include <stdio.h>

// This test requires predictable allocation failure conditions
// and the ability to extend the heap in the new_handler.
#ifdef __LLVM_MOS_SDK

// The amount of bytes used in overhead is an implementation detail.
// This amount might need to be adjusted if the implementation changes.
static constexpr size_t ALLOC_OVERHEAD = 16;

// test the allocation pattern typical of a vector.
static void testVectorAlloc() {
  void *last_alloc = nullptr;

  // If first argument is a null pointer, the behavior is the same as calling
  // malloc() with the size provided by the second argument
  const auto alloc = realloc(last_alloc, 1);
  if (!alloc) {
    puts("REALLOC NULL FAILED");
    abort();
  }
  last_alloc = alloc;

  for (unsigned char i = 1; i < 13; i += 1) {
    const auto new_alloc_sz = static_cast<size_t>(1) << i;
    const auto alloc = realloc(last_alloc, new_alloc_sz);

    // Expect that reallocating from the heap will extend the existing
    // allocation, since that is the only allocation present in the heap.
    if (alloc != last_alloc) {
      printf("REALLOC FAILED TO EXTEND EXISTING ALLOC FROM %u to %u\n",
        static_cast<size_t>(1) << (i - 1), new_alloc_sz);
      abort();
    }

    if (__heap_bytes_used() > new_alloc_sz + ALLOC_OVERHEAD) {
      puts("ALLOC OVERHEAD HIGHER THAN EXPECTED");
      abort();
    }

    last_alloc = alloc;
  }

  free(last_alloc);

  if (__heap_bytes_used() > ALLOC_OVERHEAD) {
    puts("BYTES LEAKED IN VECTOALLOC");
    abort();
  }
}

// create a scenario where a realloc must degrade to a malloc/copy/free
static void testAdjacentRealloc() {
  const auto allocLeft = static_cast<char *>(malloc(13));
  const auto allocMiddle = static_cast<char *>(malloc(14));
  const auto allocRight = static_cast<char *>(malloc(15));
  if (!(allocLeft && allocMiddle && allocRight)) {
    puts("ADJACENTS ALLOC FAILED");
    abort();
  }

  const auto distanceMiddleToRight = allocRight - allocMiddle;

  if (distanceMiddleToRight < 14 ||
      distanceMiddleToRight > (14 + ALLOC_OVERHEAD)) {
    printf("EXPECTING ADJACENT ALLOC $%p / $%p / $%p\n",
      allocLeft, allocMiddle, allocRight);
    abort();
  }

  const auto moveMiddleAlloc = realloc(allocMiddle, 42);
  if (!moveMiddleAlloc) {
    puts("REALLOC FAILED");
    abort();
  }

  if (moveMiddleAlloc == allocMiddle) {
    puts("REALLOC DID NOT MOVE WHEN IT NEEDED TO");
    abort();
  }

  if (__heap_bytes_used() > 13 + 15 + 42 + ALLOC_OVERHEAD) {
    printf("UNEXPECTED MEM USAGE AFTER REALLOC: %u", __heap_bytes_used());
    abort();
  }

  free(allocLeft);
  if (__heap_bytes_used() > 15 + 42 + ALLOC_OVERHEAD) {
    printf("UNEXPECTED MEM USAGE AFTER FIRST FREE: %u", __heap_bytes_used());
    abort();
  }

  free(moveMiddleAlloc);
  if (__heap_bytes_used() > 15 + ALLOC_OVERHEAD) {
    printf("UNEXPECTED MEM USAGE AFTER SECOND FREE: %u", __heap_bytes_used());
    abort();
  }

  free(allocRight);
  if (__heap_bytes_used() > ALLOC_OVERHEAD) {
    printf("MEM LEAKED IN TESTADJACENTREALLOC: %u", __heap_bytes_used());
    abort();
  }
}

void testReallocOOM() {
  const auto allocLeft = static_cast<char *>(malloc(13));
  const auto allocRight = static_cast<char *>(malloc(14));

  if (!(allocLeft && allocRight)) {
    abort();
  }

  const auto heapBytesInit = __heap_bytes_used();

  const auto reallocRight = realloc(allocRight, 4100);
  if (reallocRight) {
    puts("REALLOC SHOULD HAVE RETURNED NULL");
    abort();
  }

  if (__heap_bytes_used() != heapBytesInit) {
    puts("MEMORY USAGE CHANGED AFTER FAILED REALLOC");
    abort();
  }

  free(allocRight);
  free(allocLeft);

  if (__heap_bytes_used() > ALLOC_OVERHEAD) {
    puts("MEM LEAKED IN TESTREALLOCOOOM");
    abort();
  }
}

int main() {
  // allocating up to 4096 bytes
  __set_heap_limit(4096 + ALLOC_OVERHEAD);

  testVectorAlloc();

  testAdjacentRealloc();

  testReallocOOM();

  return 0;
}
#else
int main() { return 0; }
#endif
