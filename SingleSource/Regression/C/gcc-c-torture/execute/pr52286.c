/* PR tree-optimization/52286 */
// Modified by LLVM-MOS.

extern void abort (void);

int
main ()
{
#if __SIZEOF_INT__ > 2
  int a, b;
  b = (~a | 1) & -2038094497;
#else
  long a, b;
  b = (~a | 1) & -2038094497L;
#endif
  if (b >= 0)
    abort ();
  return 0;
}
