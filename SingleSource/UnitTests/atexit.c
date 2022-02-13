#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

void a() { puts("a"); }
void c() { puts("c"); }
void d() { puts("d"); }
void b() {
  puts("b");
  bool failure = false;
  failure |= atexit(c);
  failure |= atexit(d);
  if (failure) {
    puts("d");
    puts("c");
  }
}

int main(void) {
  atexit(a);
  atexit(b);
  return 0;
}
