# LLVM-MOS End-to-End Test Suite

Given its very broad scope, it's likely that quite a bit of this test suite will
*never* work on MOS. For example, it's unlikely that, say, an unaltered 7zip can
be made to fit in 64KiB.

Please see the LLVM testing infrastructure guide:

  https://llvm.org/docs/TestSuiteGuide.html

for more information on the contents of this repository.

# Running the Tests

A working (such as it is) compiler and SDK build are expected to be in
$HOME/llvm-mos and $HOME/llvm-mos-sdk, respectively. `llvm-lit` in the
the llvm-mos project should also have been built.

Use CMake to configure the SingleSource directory for MOS:

```console
$ mkdir build
$ cd build
$ cmake -DCMAKE_C_COMPILER=$HOME/llvm-mos/build/bin/clang -DARCH="MOS" -DTEST_SUITE_ARCH_FLAGS="--config $HOME/llvm-mos-sdk/build/sim.cfg" -DTEST_SUITE_SUBDIRS=SingleSource/ -DTEST_SUITE_RUN_UNDER=$HOME/llvm-mos-sdk/build/bin/sim -DTEST_SUITE_USER_MODE_EMULATION=yes -C../cmake/caches/Os.cmake -G Ninja ..
```

A variety of options can be added after "sim.cfg" to test different modes of the
compiler. Similarly, different optimization profiles can be selected using a
different CMake cache file.

Next, you can compile the tests. Most of them will have compile failures, so
disable ninja's fail-fast mode with `-k`.

```console
$ ninja -k 0
```

Finally, run the tests and produce a report.

```console
$ ../../llvm-mos/build/bin/llvm-lit -v -o results.json .
```
