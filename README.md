# LLVM-MOS End-to-End Test Suite

Given its very broad scope, it's likely that quite a bit of this test suite will
*never* work on MOS. For example, it's unlikely that, say, an unaltered 7zip can
be made to fit in 64KiB.

That being said, quite a bit of the SingleSource regression suite can fit, and
as much of that as possible has been made to pass. The remaining exclusions are
due to unsupported features or incorrect assumptions.

Please see the LLVM testing infrastructure guide:

  https://llvm.org/docs/TestSuiteGuide.html

for more information on the contents of this repository.

# Running the Tests

A SDK must be installed into the arbitrary prefix `<install-prefix>`.
`llvm-lit` in the the llvm-mos project should also have been built.

Use CMake to configure the SingleSource directory for MOS:

```console
$ mkdir build
$ cd build
$ cmake -DLLVM_MOS=<install-prefix> -C../cmake/caches/Os.cmake -C../cmake/caches/target-mos.cmake -G Ninja ..
```

To test 65C02, use this CMake line instead:

```console
$ cmake -DTEST_SUITE_ARCH_FLAGS="-mcpu=mos65c02" -DTEST_SUITE_SUBDIRS=SingleSource/ -DTEST_SUITE_RUN_UNDER="<install-prefix>/bin/mos-sim --cmos" -C../cmake/caches/Os.cmake -C../cmake/caches/target-mos.cmake -G Ninja ..
```

A variety of options can be added via `-DTEST_SUITE_ARCH_FLAGS=` to test
different modes of the compiler. Similarly, different optimization profiles
can be selected using a different CMake cache file.

Next, compile the tests:

```console
$ ninja
```

Finally, run the tests:

```console
$ <llvm-mos-build-dir>/bin/llvm-lit -v .
```
