#include <stdio.h>

__attribute__((constructor(200))) void first() { puts("FIRST"); }
__attribute__((constructor(1000))) void second() { puts("SECOND"); }

int main() { return 0; }
