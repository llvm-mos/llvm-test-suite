# LLVM-MOS End-to-End Test Suite

Given its very broad scope, it's likely that quite a bit of this test suite will
*never* work on MOS. For example, it's unlikely that, say, an unaltered 7zip can
be made to fit in 64KiB.

As testing progresses, sections of the test suite known to work on MOS will be
carved out in this document, and instructions to run the supported subset will
be provided. For now, this subset is empty.

Please see the LLVM testing infrastructure guide:

  https://llvm.org/docs/TestSuiteGuide.html

for more information on the contents of this repository.

# CMake Command

The following command can at least allow some of the single-source test cases to
compile and run (fail). A working (such as it is) compiler and SDK build are
expected to be in $HOME/llvm-mos and $HOME/llvm-mos-sdk, respectively.

```console
$ cmake -DCMAKE_C_COMPILER=$HOME/llvm-mos/build/bin/clang -DARCH="MOS" -DTEST_SUITE_ARCH_FLAGS="--target=mos --config $HOME/llvm-mos-sdk/build/sim.cfg" -DTEST_SUITE_SUBDIRS=SingleSource/ -DTEST_SUITE_RUN_UNDER=$HOME/llvm-mos-sdk/build/bin/sim -DTEST_SUITE_USER_MODE_EMULATION=yes -C../cmake/caches/Os.cmake -G Ninja .
```
