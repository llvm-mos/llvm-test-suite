#include <stdio.h>
#include <stdlib.h>

void atexit_handler(void) {
  printf("Atexit handlers incorrectly called.\n");
}

void first_handler(void) {
  printf("First quick_exit handler to be registered.\n");
}

void second_handler(void) {
  printf("Second quick_exit handler to be registered.\n");
}

int main(void) {
  atexit(atexit_handler);
  at_quick_exit(first_handler);
  at_quick_exit(second_handler);
  quick_exit(42);
  printf("quick_exit returned to its caller.\n");
  return EXIT_FAILURE;
}
