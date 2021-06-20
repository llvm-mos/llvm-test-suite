// Modified by LLVM-MOS.
// PR594

struct X {
  int Q :6;
   int A : 4;
   long Z : 22;
};

void test(struct X *P, int A) {
   P->A = A;
}

extern int printf(const char *str, ...);

int main() {
  union {
    long Y;
    struct X Z;
  } U;
  U.Y = ~0;
  test(&U.Z, 0);
  printf("%lx\n", U.Y);
  return 0;
}
