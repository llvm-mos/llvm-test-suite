#include <initializer_list>
#include <stdint.h>
#include <stdio.h>

namespace {
template <class T> struct small_array {

  small_array(std::initializer_list<T> init_list)
      : data{new T[init_list.size()]}, sz{init_list.size()} {
    // demonstrate usage of free-function std::begin() / std::end()
    T *dest_iter = data;
    for (auto iter = std::begin(init_list); iter != std::end(init_list);
         ++iter, ++dest_iter) {
      *dest_iter = *iter;
    }
  }

  ~small_array() { delete[] data; }

  const T *begin() const noexcept { return data; }
  const T *end() const noexcept { return data + sz; }

  T *data;
  std::size_t sz;
};

} // namespace

int main() {
  auto auto_initializer_list = {10, 11, 12};

  printf("AUTO INITIALIZER LIST WITH SIZE %u\n", auto_initializer_list.size());

  small_array<const char *> some_strings{"ITEM 1", "ITEM 2", "ITEM 3",
                                         "ITEM 4"};

  for (auto str : some_strings) {
    puts(str);
  }

  // verifies combination of:
  // automatically instantiated initializer_list
  // foreach loop that uses begin() / end() on the implicit initializer list.
  for (auto str : {"FOREACH + INIT LIST", ".1.", ".2.", ".3.", ".4."}) {
    puts(str);
  }

  return 0;
}
