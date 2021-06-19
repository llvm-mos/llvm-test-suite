/* PR rtl-optimization/45695 */
// Modified by LLVM-MOS.

extern void abort (void);

__attribute__((noinline)) void
g (int x)
{
}

__attribute__((noinline)) int
f (int a, int b, int d)
{
  int r = -1;
  b += d;
  if (d == a)
    r = b - d;
  g (b);
  return r;
}

int
main (void)
{
  int l = 0;
  if (f (l + 0, l + 1, l + 4) != -1)
    abort ();
  if (f (l + 4, l + 1, l + 4) != 1)
    abort ();
  return 0;
}
