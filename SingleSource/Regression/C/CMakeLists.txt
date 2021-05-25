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

    # Requires floating point printf.
    2008-01-07-LongDouble.c
    callargs.c

    # Requires long long int print.
    PR1386.c

    # Requires GCC constructor/destructor extension.
    ConstructorDestructorAttributes.c
  )
endif()

list(APPEND LDFLAGS -lm)
llvm_singlesource(PREFIX "Regression-C-")
