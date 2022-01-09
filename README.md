# LLVM-MOS End-to-End Test Suite

Given its very broad scope, it's likely that quite a bit of this test suite will
*never* work on MOS. For example, it's unlikely that, say, an unaltered 7zip can
be made to fit in 64KiB.

Please see the LLVM testing infrastructure guide:

  https://llvm.org/docs/TestSuiteGuide.html

for more information on the contents of this repository.

# Running the Tests

A working (such as it is) compiler and SDK build are expected to be installed
into the same arbitrary prefix `<install-prefix>`. `llvm-lit` in the the
llvm-mos project should also have been built.

Use CMake to configure the SingleSource directory for MOS:

```console
$ mkdir build
$ cd build
$ cmake -DCMAKE_C_COMPILER=<install-prefix>/bin/mos-sim-clang -DCMAKE_C_COMPILER=<install-prefix>/bin/mos-sim-clang++ -DARCH=MOS -DTEST_SUITE_SUBDIRS=SingleSource/ -DTEST_SUITE_RUN_UNDER=<install-prefix>/bin/mos-sim -DTEST_SUITE_USER_MODE_EMULATION=yes -DTEST_SUITE_LLVM_SIZE="" -DCMAKE_STRIP="" -C../cmake/caches/Os.cmake -G Ninja ..
```

To test 65C02, use this CMake line instead:

```console
$ cmake -DCMAKE_C_COMPILER=<install-prefix>/bin/mos-sim-clang -DCMAKE_C_COMPILER=<install-prefix>/bin/mos-sim-clang++ -DARCH=MOS -DTEST_SUITE_ARCH_FLAGS="-mcpu=mos65c02" -DTEST_SUITE_SUBDIRS=SingleSource/ -DTEST_SUITE_RUN_UNDER="<install-prefix>/bin/mos-sim --cmos" -DTEST_SUITE_USER_MODE_EMULATION=yes -DTEST_SUITE_LLVM_SIZE="" -DCMAKE_STRIP="" -C../cmake/caches/Os.cmake -G Ninja ..
```

A variety of options can be added via `-DTEST_SUITE_ARCH_FLAGS=` to test
different modes of the compiler. Similarly, different optimization profiles
can be selected using a different CMake cache file.

Next, you can compile the tests. 

```console
$ ninja
```

Finally, run the tests:

```console
$ <llvm-mos-build-dir>/bin/llvm-lit -v .
```
