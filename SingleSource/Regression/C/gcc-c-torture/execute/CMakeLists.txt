
if(NOT ARCH STREQUAL "MOS")
  add_subdirectory(ieee)
endif()

# GCC C Torture Suite is conventionally run without warnings
list(APPEND CFLAGS -Wno-implicit-int -Wno-int-conversion -Wno-implicit-function-declaration -w)

set(TestsToSkip)

##
## Main Test Blacklist for Clang
##

# Tests with features unsupported by Clang (usually GCC extensions)
# (Big list of naughty tests)
file(GLOB UnsupportedTests CONFIGURE_DEPENDS
  # GCC Extension: Nested functions
  20000822-1.c
  20010209-1.c
  20010605-1.c
  20030501-1.c
  20040520-1.c
  20061220-1.c
  20090219-1.c
  920415-1.c
  920428-2.c
  920501-7.c
  920612-2.c
  920721-4.c
  921017-1.c
  921215-1.c
  931002-1.c
  comp-goto-2.c
  nest-align-1.c
  nest-stdar-1.c
  nestfunc-1.c
  nestfunc-2.c
  nestfunc-3.c
  nestfunc-5.c
  nestfunc-6.c
  nestfunc-7.c
  pr22061-3.c
  pr22061-4.c
  pr24135.c
  pr51447.c
  pr71494.c

  # Variable length arrays in structs
  20020412-1.c
  20040308-1.c
  20040423-1.c
  20041218-2.c
  20070919-1.c
  align-nest.c
  pr41935.c
  pr82210.c

  # Initialization of flexible array member
  pr28865.c

  # Initialization of union is not required to initialize padding.
  pr19687.c

  # GCC Extension: __builtin_*
  pr39228.c          # __builtin_isinff, __builtin_isinfl
  pr47237.c          # __builtin_apply, __builtin_apply_args
  pr85331.c          # __builtin_shuffle
  va-arg-pack-1.c    # __builtin_va_arg_pack

  # Clang does not support 'DD' suffix on floating constant
  pr80692.c

  # Test requires compiler to recognise llabs without including <inttypes.h> -
  # clang will only recognise this function if the header is included.
  20021127-1.c

  # Tests __attribute__((noinit))
  noinit-attribute.c

  # We are unable to parse the dg-additional-options for this test, which is
  # required for it to work (we discard any with `target`, but we need the
  # define for this test)
  20101011-1.c

  # The following rely on C Undefined Behavior

  # Test relies on UB around (float)INT_MAX
  20031003-1.c

  # UB: Expects very specific behavior around setjmp/longjmp and allocas, which
  # clang is not obliged to replicate.
  pr64242.c

  # UB: Creates two `restrict` pointers that alias in the same scope.
  pr38212.c

  # UB: Each comparisons in (cyx != cyy || mpn_cmp (dx, dy, size + 1) != 0 ||
  #     dx[size] != 0x12345678) is UB on its own.
  921202-1.c

  # The following all expect very specific optimiser behavior from the compiler

  # __builtin_return_address(n) with n > 0 not guaranteed to give expected result
  20010122-1.c

  # Expects gnu89 inline behavior
  20001121-1.c
  20020107-1.c
  930526-1.c
  961223-1.c
  980608-1.c
  bcp-1.c
  loop-2c.c
  p18298.c
  restrict-1.c
  unroll-1.c
  va-arg-7.c
  va-arg-8.c

  # Clang at O0 does not work out the code referencing the undefined symbol can
  # never be executed
  medce-1.c

  # Expects that function is always inlined
  990208-1.c

  # pragma optimize("-option") is ignored by Clang
  alias-1.c
  pr79043.c

  # The following all expect very specific optimiser behavior from the compiler
  # around __printf_chk and friends.
  fprintf-chk-1.c
  printf-chk-1.c
  vfprintf-chk-1.c
  vprintf-chk-1.c

  # Clang at -O0 does not enable -finstrument-functions
  # (https://bugs.llvm.org/show_bug.cgi?id=49143)
  eeprof-1.c

  # Size of array element is not a multiple of its alignment.
  pr36093.c
  pr43783.c
)
list(APPEND TestsToSkip ${UnsupportedTests})

# Tests where clang currently has bugs or issues
file(GLOB FailingTests CONFIGURE_DEPENDS

  # Handling of bitfields is different between clang and GCC:
  # http://lists.llvm.org/pipermail/llvm-dev/2017-October/118507.html
  # https://gcc.gnu.org/ml/gcc/2017-10/msg00192.html
  # http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1260.htm
  bitfld-3.c
  bitfld-5.c
  pr32244-1.c
  pr34971.c

  # This causes a stacktrace on x86 in X86TargetLowering::LowerCallTo
  pr84169.c

  # clang complains the array is too large
  991014-1.c

  # __builtin_setjmp/__builtin_longjmp are interacting badly with optimisation
  pr60003.c
)
list(APPEND TestsToSkip ${FailingTests})

##
## Tests that require extra CFLAGS in Clang
##

# Tests that require -fwrapv
file(GLOB TestRequiresFWrapV CONFIGURE_DEPENDS
  # Test relies on undefined signed overflow behavior (int foo - INT_MIN).
  20040409-1.c
  20040409-2.c
  20040409-3.c
  950704-1.c
)

# Tests that require -Wno-return-type
file(GLOB TestRequiresWNoReturnType CONFIGURE_DEPENDS
  # Non-void function must return a value
  920302-1.c
  920501-3.c
  920728-1.c
)

# Tests that require libm (-lm ldflag)
file(GLOB TestRequiresLibM CONFIGURE_DEPENDS
  980709-1.c
  float-floor.c
  complex-5.c  # -lm needed on FreeBSD
)

##
## Architecture-specific Test Blacklists
##


# Tests that require mmap
include(CheckSymbolExists)
CHECK_SYMBOL_EXISTS("mmap" "sys/types.h;sys/mman.h" HAVE_MMAP)
if (NOT HAVE_MMAP)
  file(GLOB MMapTests
    loop-2f.c
    loop-2g.c
  )
  list(APPEND TestsToSkip ${MMapTests})
endif()

# x86-only Tests
if(NOT ARCH MATCHES "x86")
  file(GLOB X86OnlyTests CONFIGURE_DEPENDS
    990413-2.c
  )

  list(APPEND TestsToSkip ${X86OnlyTests})
endif()

# MIPS Test Blacklist
if(ARCH MATCHES "Mips")
  file(GLOB MipsTestsToSkip CONFIGURE_DEPENDS
    # No backend support for __builtin_longjmp/__builtin_setjmp
    built-in-setjmp.c
    pr84521.c
    # Triggers "return address can be determined only for current frame"
    20030323-1.c
    # These test cases compiled very long due PR43263.
    pr20621-1.c
    pr23135.c
    pr28982b.c
  )

  # MIPS 32-bit Test Blacklist
  if(NOT MIPS_IS_MIPS64_ENABLED)
    file(GLOB Mips32TestsToSkip CONFIGURE_DEPENDS
      # No support for __int128 on MIPS 32-bit
      pr84748.c
    )

    list(APPEND MipsTestsToSkip ${Mips32TestsToSkip})
  endif()

  list(APPEND TestsToSkip ${MipsTestsToSkip})
endif()

# RISC-V Test Blacklist
if(ARCH MATCHES "riscv")
  file(GLOB RISCVTestsToSkip CONFIGURE_DEPENDS
    # No backend support for __builtin_longjmp/__builtin_setjmp
    built-in-setjmp.c
    pr84521.c
  )

  # RISC-V 32-bit Test Blacklist
  if (ARCH MATCHES "riscv32")
    file(GLOB RISCV32TestsToSkip CONFIGURE_DEPENDS
      # No support for __int128 on rv32
      pr84748.c
    )

    list(APPEND RISCVTestsToSkip ${RISCV32TestsToSkip})
  endif()

  list(APPEND TestsToSkip ${RISCVTestsToSkip})
endif()

# SystemZ Test Blacklist
if(ARCH MATCHES "SystemZ")
  file(GLOB SystemZTestsToSkip CONFIGURE_DEPENDS
    # error: __builtin_longjmp is not supported for the current target
    # error: __builtin_setjmp is not supported for the current target
    built-in-setjmp.c
    pr84521.c
    # Triggers "Unsupported stack frame traversal count"
    20030323-1.c
  )

  list(APPEND TestsToSkip ${SystemZTestsToSkip})
endif()

# AArch64 Test Blacklist
if(ARCH MATCHES "AArch64")
  file(GLOB AArch64TestsToSkip CONFIGURE_DEPENDS
    # error: __builtin_longjmp is not supported for the current target
    # error: __builtin_setjmp is not supported for the current target
    built-in-setjmp.c
    pr84521.c
    # Triggers an assertion failure at at least -O3
    va-arg-22.c
    # Triggers an assertion failure in RegBankSelect pass at -O0 (GlobalIsel)
    pr71626-1.c
    pr71626-2.c
  )

  list(APPEND TestsToSkip ${AArch64TestsToSkip})
endif()

# ARM Test Blacklist
if(ARCH MATCHES "ARM")
  file(GLOB ARMTestsToSkip CONFIGURE_DEPENDS
    # No support for __int128 on ARM 32-bit
    pr84748.c

    # With some command line options, linking fails with:
    # pr89434.c:(.text+0xac): undefined reference to `__mulodi4'
    pr89434.c

    # Fails in the front end with:
    # clang: llvm/include/llvm/Support/Casting.h:269: typename
    #   cast_retty<X, Y *>::ret_type llvm::cast(Y *) [X = llvm::PointerType,
    #   Y = llvm::Type]: Assertion `isa<X>(Val) && "cast<Ty>() argument of
    #   incompatible type!"' failed.
    va-arg-22.c
  )

  list(APPEND TestsToSkip ${ARMTestsToSkip})
endif()

if(ARCH STREQUAL "MOS")
  file(GLOB MOSTestsToSkip CONFIGURE_DEPENDS
    # Permanently disabled tests.
    # ---------------------------

    # Requires 32-bit int.
    20020404-1.c
    20021024-1.c
    20040629-1.c
    20040703-1.c
    20040705-1.c
    20040705-2.c
    20040709-1.c
    20040709-2.c
    20040709-3.c
    20040811-1.c
    20050316-1.c
    20061101-1.c
    20081117-1.c
    20120919-1.c
    20180131-1.c
    bitfld-4.c
    bitfld-6.c
    bitfld-7.c
    bswap-2.c
    cmpsi-2.c
    ipa-sra-2.c
    pr19689.c
    pr29797-1.c
    pr31448.c
    pr37573.c
    pr38151.c
    pr43220.c
    pr45262.c
    pr51581-1.c
    pr51581-2.c
    pr52979-1.c
    pr52979-2.c
    pr56799.c
    pr57124.c
    pr57344-2.c
    pr58570.c
    pr60822.c
    pr66556.c
    pr67781.c
    pr68648.c
    pr70602.c
    pr7284-1.c
    pr78170.c
    pr89195.c
    shiftdi.c
    usmul.c
    vrp-5.c
    vrp-6.c

    # Requires vector types.
    20050316-1.c
    20050604-1.c
    20060420-1.c
    pr23135.c
    pr53645-2.c
    pr53645.c
    pr60960.c
    pr65427.c
    pr70903.c
    pr85169.c
    scal-to-vec2.c
    simd-1.c
    simd-2.c
    simd-4.c
    simd-5.c
    simd-6.c


    # Requires >=64K objects.
    20030209-1.c
    20040805-1.c
    920410-1.c
    921113-1.c
    921208-2.c
    930106-1.c
    960521-1.c
    980605-1.c
    ipa-sra-2.c
    memcpy-1.c
    pr20621-1.c
    pr28982b.c

    # Requires __builtin_prefetch.
    builtin-prefetch-1.c
    builtin-prefetch-2.c
    builtin-prefetch-3.c
    builtin-prefetch-4.c
    builtin-prefetch-5.c
    builtin-prefetch-6.c

    # Requires __int128
    pr84748.c

    # Requires __builtin_trap
    20020418-1.c

    # Requires silent inline asm operand truncation.
    20030222-1.c
    pr52286.c

    # Has undefined behavior?!?
    string-opt-18.c

    # Bizarre use of computed goto (LLVM bug 45818).
    920302-1.c

    # Temporarily disabled tests.
    # ---------------------------

    # Requires floating point.
    20000223-1.c
    20000603-1.c
    20000605-1.c
    20000731-1.c
    20001017-1.c
    20001121-1.c
    20010118-1.c
    20010605-2.c
    20011217-1.c
    20020213-1.c
    20020225-2.c
    20020227-1.c
    20020314-1.c
    20020321-1.c
    20020411-1.c
    20020413-1.c
    20020720-1.c
    20021118-2.c
    20021120-1.c
    20030125-1.c
    20030209-1.c
    20030216-1.c
    20030714-1.c
    20030910-1.c
    20030914-1.c
    20031003-1.c
    20031214-1.c
    20040208-1.c
    20040709-1.c
    20040709-2.c
    20040831-1.c
    20041113-1.c
    20050121-1.c
    20050215-1.c
    20050316-2.c
    20050604-1.c
    20060420-1.c
    20070614-1.c
    20071030-1.c
    20080117-1.c
    20080502-1.c
    20080529-1.c
    20081103-1.c
    20100708-1.c
    20120919-1.c
    920501-8.c
    920625-1.c
    920710-1.c
    920721-2.c
    920929-1.c
    921013-1.c
    921019-2.c
    921112-1.c
    921113-1.c
    921124-1.c
    921208-1.c
    921208-2.c
    930106-1.c
    930513-1.c
    930603-1.c
    930608-1.c
    930614-1.c
    930614-2.c
    930622-2.c
    930628-1.c
    930702-1.c
    930818-1.c
    930930-2.c
    941021-1.c
    960215-1.c
    960405-1.c
    960512-1.c
    960513-1.c
    961223-1.c
    980205.c
    980604-1.c
    980605-1.c
    980709-1.c
    990117-1.c
    990127-2.c
    990413-2.c
    990826-0.c
    990829-1.c
    991019-1.c
    991030-1.c
    991228-1.c
    alias-1.c
    align-2.c
    builtin-types-compatible-p.c
    cbrt.c
    CMakeLists.txt
    cmpsf-1.c
    comp-goto-1.c
    complex-1.c
    complex-2.c
    complex-3.c
    complex-4.c
    complex-5.c
    complex-6.c
    complex-7.c
    conversion.c
    cvt-1.c
    float-floor.c
    floatunsisf-1.c
    func-ptr-1.c
    gofast.c
    ipa-sra-1.c
    loop-8.c
    loop-ivopts-1.c
    lto-tbaa-1.c
    memcpy-2.c
    memset-1.c
    memset-2.c
    memset-3.c
    multi-ix.c
    nest-stdar-1.c
    postmod-1.c
    pr15262-1.c
    pr15262-2.c
    pr15262.c
    pr23324.c
    pr23941.c
    pr28982a.c
    pr28982b.c
    pr29156.c
    pr29798.c
    pr35456.c
    pr35800.c
    pr36034-1.c
    pr36034-2.c
    pr36343.c
    pr38969.c
    pr39228.c
    pr39501.c
    pr40493.c
    pr42248.c
    pr42691.c
    pr44468.c
    pr44575.c
    pr44683.c
    pr44942.c
    pr47538.c
    pr49218.c
    pr49644.c
    pr56205.c
    pr58574.c
    pr59643.c
    pr64979.c
    pr66233.c
    pr67929_1.c
    pr68390.c
    pr79043.c
    pr79354.c
    pr85331.c
    regstack-1.c
    scal-to-vec1.c
    scal-to-vec3.c
    stdarg-1.c
    stdarg-2.c
    stdarg-3.c
    stdarg-4.c
    strcmp-1.c
    strcpy-1.c
    strct-pack-1.c
    strlen-1.c
    strncmp-1.c
    struct-aliasing-1.c
    struct-ret-1.c
    va-arg-12.c
    va-arg-15.c
    va-arg-16.c
    va-arg-17.c
    va-arg-18.c
    va-arg-26.c
    va-arg-5.c
    va-arg-6.c
    va-arg-7.c
    va-arg-pack-1.c
    va-arg-trap-1.c

    # Requires __builtin_return_address.
    20010122-1.c
    20030323-1.c
    20030811-1.c
    pr17377.c

    # Requires __builtin_frame_address.
    frame-address.c

    # Requires built-in-setjmp
    built-in-setjmp.c
    pr84521.c

    # Requires FILE*
    fprintf-1.c
    fprintf-2.c
    printf-2.c
    user-printf.c
    vfprintf-1.c

    # Requires qsort
    pr34456.c

    # Requres "g" inline asm contraint.
    pr40022.c
    pr51933.c
    pr56866.c
    pr65053-1.c
    pr65053-2.c
    pr68328.c
    pr78726.c
    pr81588.c
    pr82954.c
    pr88904.c
    pr65956.c
    stkalign.c
  )

  list(APPEND TestsToSkip ${MOSTestsToSkip})
  if (CMAKE_BUILD_TYPE STREQUAL Debug AND CMAKE_C_FLAGS_DEBUG MATCHES -O0)
    file(GLOB MOSO0TestsToSkip CONFIGURE_DEPENDS
      # Too big.
      strlen-4.c
      strlen-5.c

      # Too much stack.
      20000412-2.c
      nestfunc-4.c

      # Too much reg pressure.
      builtin-bitops-1.c
    )
    list(APPEND TestsToSkip ${MOSO0TestsToSkip})
  endif()
endif()

# Darwin Test Blacklist
if(TARGET_OS STREQUAL "Darwin")
  file(GLOB DarwinTestsToSkip CONFIGURE_DEPENDS
    # error: aliases are not supported on darwin
    alias-2.c
    alias-3.c
    alias-4.c
    # Undefined symbols for architecture x86_64:
    # "_printf_float", referenced from:
    # -u command line option
    930513-1.c
    920501-8.c
  )
  list(APPEND TestsToSkip ${DarwinTestsToSkip})
endif()

##
## Test target setup
##

file(GLOB TestFiles CONFIGURE_DEPENDS
  *.c
)
foreach(TestToSkip ${TestsToSkip})
  list(REMOVE_ITEM TestFiles ${TestToSkip})
endforeach()

foreach(File ${TestFiles})
  set(MaybeCFlags)
  set(MaybeLDFlags)

  # Add Test-specific CFLAGS/LDFLAGS here

  if(${File} IN_LIST TestRequiresLibM)
    list(APPEND MaybeLDFlags "-lm")
  endif()

  if(${File} IN_LIST TestRequiresWNoReturnType)
    list(APPEND MaybeCFlags "-Wno-return-type")
  endif()

  if(${File} IN_LIST TestRequiresFWrapV)
    list(APPEND MaybeCFlags "-fwrapv")
  endif()

  # Add Test Target
  gcc_torture_execute_test(${File}
                           PREFIX "GCC-C-execute"
                           CFLAGS ${MaybeCFlags}
                           LDFLAGS ${MaybeLDFlags})
endforeach()
