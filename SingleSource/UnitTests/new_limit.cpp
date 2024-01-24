#include <new>
#include <stdint.h>
#include <stdio.h>
#define _MOS_SOURCE 1
#include <stdlib.h>

namespace {
// recreate std::move and std::unique_ptr<T[]>
template <class T> T &&move(T &src) { return static_cast<T &&>(src); }

template <class T> struct unique_ptr {
  unique_ptr(std::size_t N) : m_ptr{new T[N]} {}
  unique_ptr(std::size_t N, std::nothrow_t) : m_ptr{new (std::nothrow) T[N]} {}
  unique_ptr() = default;
  unique_ptr(const unique_ptr &) = delete;

  unique_ptr(unique_ptr &&src) : m_ptr{src.m_ptr} { src.m_ptr = nullptr; }

  unique_ptr &operator=(unique_ptr &&src) {
    delete[] m_ptr;
    m_ptr = src.m_ptr;
    src.m_ptr = nullptr;
    return *this;
  }

  unique_ptr &operator=(const unique_ptr &) = delete;

  T &operator[](uint8_t idx) { return m_ptr[idx]; }
  T &operator[](std::size_t idx) { return m_ptr[idx]; }
  const T &operator[](uint8_t idx) const {
    return const_cast<unique_ptr *>(this)->operator[](idx);
  }
  T &operator[](std::size_t idx) const {
    return const_cast<unique_ptr *>(this)->operator[](idx);
  }

  ~unique_ptr() { delete[] m_ptr; }
  T *get() const { return m_ptr; }
  operator bool() const { return m_ptr != nullptr; }
  T *m_ptr = nullptr;
};

template <class T, size_t N> constexpr size_t static_size(const T (&)[N]) {
  return N;
}
} // namespace

extern std::byte __heap_start;

int main() {

#ifdef __LLVM_MOS_SDK
  __set_heap_limit(4096);
#endif

  { unique_ptr<char> an_alloc{500}; }

  {
    unique_ptr<char> an_alloc{40000, std::nothrow};
    if (an_alloc) {
      abort();
    }
  }

  static constexpr uint8_t alloc_sizes[] = {1, 4, 16, 7, 35, 100, 3, 9};
  static constexpr auto ALLOC_COUNT = static_size(alloc_sizes);
  static unique_ptr<uint8_t> allocations[ALLOC_COUNT] = {};

  for (uint8_t i = 0; i < ALLOC_COUNT; i += 1) {
    const auto alloc_sz = alloc_sizes[i];
    unique_ptr<uint8_t> new_alloc{alloc_sz};

    for (uint8_t j = 0; j < alloc_sz; j += 1) {
      new_alloc[j] = alloc_sz;
    }
    allocations[i] = move(new_alloc);
  }

  // dealloc in a different order than alloc
  static uint8_t dealloc_order[ALLOC_COUNT] = {4, 6, 2, 0, 3, 1, 7, 5};

  for (uint8_t i = 0; i < ALLOC_COUNT; i += 1) {
    const auto test_alloc_idx = dealloc_order[i];
    const unique_ptr<uint8_t> alloc_to_test = move(allocations[test_alloc_idx]);
    const auto test_alloc_size = alloc_sizes[test_alloc_idx];

    for (uint8_t j = 0; j < test_alloc_size; j += 1) {
      if (alloc_to_test[j] != test_alloc_size) {
        printf("UNEXPECTED VALUE STORED IN HEAP: EXPECT %u, ACTUAL %u\n",
               static_cast<unsigned>(test_alloc_size),
               static_cast<unsigned>(alloc_to_test[j]));
        abort();
      }
    }
  }

#ifdef __LLVM_MOS_SDK
  {
    // heap limiting tests
    // Use the heap to allocate pointers to some allocations:
    constexpr auto ALLOC_COUNT = 1000;
    unique_ptr<std::size_t *> vector_of_ptrs{ALLOC_COUNT, std::nothrow};

    for (std::size_t j = 0; j < ALLOC_COUNT; j += 1) {
      vector_of_ptrs[j] = nullptr;
    }

    std::size_t i = 0;

    for (; i < ALLOC_COUNT; i += 1) {
      auto int_ptr = new (std::nothrow) std::size_t{i};
      if (!int_ptr) {
        break;
      }

      vector_of_ptrs[i] = int_ptr;
    }

    if (::heap_bytes_used() > 4096) {
      printf("BYTES IN USE: %u\n", ::heap_bytes_used());
      abort();
    }

    const auto new_limit = ::heap_limit() + 8192;
    ::set_heap_limit(new_limit);
    for (; i < ALLOC_COUNT; i += 1) {
      vector_of_ptrs[i] = new std::size_t{i};
    }

    for (i = 0; i < ALLOC_COUNT; i += 1) {
      delete vector_of_ptrs[i]; // manual delete required for raw ptrs.
    }
  }

  // At the end of the program, there should only be a few bytes in use by the
  // heap's internal data structures.  For example, assuming the heap combined
  // all freed blocks of memory, there should just be the overhead of a single
  // block description
  if (::heap_bytes_used() > 32) {
    puts("TOO MUCH OVERHEAD IN HEAP\n");
    abort();
  }
#endif

  return 0;
}
