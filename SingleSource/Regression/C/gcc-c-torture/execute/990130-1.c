// Modified by LLVM-MOS.
int count = 0;
int dummy;

static int *
bar(void)
{
  ++count;
  return &dummy;
}

static void
foo(void)
{
  bar();
}

main()
{
  foo();
  if (count != 1)
    abort();
  exit(0);
}
