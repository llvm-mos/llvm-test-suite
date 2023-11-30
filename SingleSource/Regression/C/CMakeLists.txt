if(ARCH MATCHES "x86" OR ARCH MATCHES "riscv" OR
   ARCH MATCHES "SystemZ" OR ARCH MATCHES "Mips" OR
   ARCH MATCHES "AArch64" OR ARCH MATCHES "ARM" OR
   ARCH MATCHES "MOS" OR ARCH MATCHES "LoongArch")
  add_subdirectory(gcc-c-torture)
endif()

if (ARCH STREQUAL "MOS")
  file(GLOB Source RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c)
  list(REMOVE_ITEM Source
    # Permanently disabled tests.
    # ---------------------------

    # Requires large stack.
    test_indvars.c

    # Too slow
    bigstack.c

    # Temporarily disabled tests.
    # ---------------------------

    # Requires indirect branch.
    2004-03-15-IndirectGoto.c

    # Requires fprintf.
    PR491.c

    # Requires atoi.
    badidx.c
    sumarraymalloc.c

    # Requires atof.
    casts.c

    # Requires fenv.h.
    uint64_to_float.c

    # Requires large whole number float printf
    2003-10-12-GlobalVarInitializers.c

    # Requires argc/argv.
    2005-05-06-LongLongSignedShift.c
  )
endif()

list(APPEND LDFLAGS -lm)
list(APPEND CFLAGS -Wno-implicit-int)
llvm_singlesource(PREFIX "Regression-C-")

if(ARCH STREQUAL "MOS")
  target_link_libraries(Regression-C-2003-05-21-UnionBitfields PRIVATE printf_flt)
  target_link_libraries(Regression-C-2003-05-21-UnionTest PRIVATE printf_flt)
  target_link_libraries(Regression-C-2008-01-07-LongDouble PRIVATE printf_flt)
  target_link_libraries(Regression-C-callargs PRIVATE printf_flt)
  target_link_libraries(Regression-C-matrixTranspose PRIVATE printf_flt)
endif()
