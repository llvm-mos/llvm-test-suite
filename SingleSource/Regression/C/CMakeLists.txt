if(ARCH MATCHES "x86" OR ARCH MATCHES "riscv" OR
   ARCH MATCHES "SystemZ" OR ARCH MATCHES "Mips" OR
   ARCH MATCHES "AArch64" OR ARCH MATCHES "ARM" OR
   ARCH MATCHES "MOS")
  add_subdirectory(gcc-c-torture)
endif()

if (ARCH MATCHES "MOS")
  file(GLOB Source RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c)
  list(REMOVE_ITEM Source
    # Permanently disabled tests.
    # ---------------------------

    # Depends on 32-bit integers.
    2003-05-21-BitfieldHandling.c
    2003-05-21-UnionBitfields.c
    2003-05-21-UnionTest.c
    2003-10-12-GlobalVarInitializers.c

    # Temporarily disabled tests.
    # ---------------------------

    # Requires indirect branch.
    2004-03-15-IndirectGoto.c

    # Requires floating point printf.
    2008-01-07-LongDouble.c
    bigstack.c
    callargs.c
    casts.c

    # Requires long long int printf.
    PR1386.c
    2005-05-06-LongLongSignedShift.c

    # Requires fprintf.
    PR491.c

    # Requires GCC constructor/destructor extension.
    ConstructorDestructorAttributes.c

    # Requires malloc.
    badidx.c
  )
endif()

list(APPEND LDFLAGS -lm)
llvm_singlesource(PREFIX "Regression-C-")
