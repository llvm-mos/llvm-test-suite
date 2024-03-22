
include(CheckCCompilerFlag)

add_subdirectory(C++11)
add_subdirectory(SignlessTypes)
if(NOT ARCH STREQUAL "MOS")
  add_subdirectory(Float)
  add_subdirectory(Matrix)
  add_subdirectory(Threads)
  add_subdirectory(Vector)
  add_subdirectory(Vectorizer)
  add_subdirectory(X86)
  add_subdirectory(AArch64)
endif()

list(APPEND CFLAGS -Wno-implicit-function-declaration -Wno-implicit-int)

# FIXME: Disable SJLJ tests for now, until EH edges are represented.
# add_subdirectory(SetjmpLongjmp)
if(TARGET_OS STREQUAL "Darwin")
  add_subdirectory(ObjC)
  add_subdirectory(ObjC++)
endif()

file(GLOB Source RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c *.cpp)
if(TARGET_OS STREQUAL "AIX" AND ARCH STREQUAL "PowerPC")
  list(REMOVE_ITEM Source 2007-04-25-weak.c)
  if (NOT PPC_IS_PPC64_ENABLED)
    list(REMOVE_ITEM Source AtomicOps.c)
  endif()
endif()
if(TARGET_OS STREQUAL "Darwin")
  list(REMOVE_ITEM Source 2007-04-25-weak.c)
  if(ARCH STREQUAL "PowerPC")
    list(REMOVE_ITEM Source AtomicOps.c)
  endif()
endif()
if(ARCH STREQUAL "AArch64")
  list(REMOVE_ITEM Source blockstret.c)
endif()
if(ARCH STREQUAL "Mips")
  add_subdirectory(Mips)
  # Only MIPS 64-bit supports 8 bytes atomic operations.
  if(NOT MIPS_IS_MIPS64_ENABLED)
    list(REMOVE_ITEM Source AtomicOps.c)
  endif()
endif()
if(ARCH STREQUAL "XCore")
  list(REMOVE_ITEM Source AtomicOps.c)
  list(REMOVE_ITEM Source initp1.cpp)
endif()
if(NOT ARCH STREQUAL "x86")
  list(REMOVE_ITEM Source
    ms_struct-bitfield.c
    ms_struct-bitfield-1.c
    ms_struct-bitfield-init.c
    ms_struct-bitfield-init-1.c
    ms_struct_pack_layout.c
    ms_struct_pack_layout-1.c
  )
endif()
if(ARCH STREQUAL "MOS")
  list(REMOVE_ITEM Source
    # Permanently disabled tests.
    # ---------------------------

    # Requires 32-bit int.
    2002-10-13-BadLoad.c
    2003-05-26-Shorts.c
    2003-07-09-LoadShorts.c

    # Requires libc++
    matrix-types-spec.cpp

    # Requires <strings.h>
    2005-05-11-Popcount-ffs-fls.c

    # Requires atomic operations
    AtomicOps.c

    # Undefined behavior?!
    2020-01-06-coverage-009.c

    # Temporarily disabled tests.
    # ---------------------------

    # Requires printing large floating point whole numbers
    2005-05-12-Int64ToFP.c
    2005-07-17-INT-To-FP.c

    # Requires scanf.
    2004-11-28-GlobalBoolLayout.c

    # Requires argc/argv.
    2003-05-31-LongShifts.c
    2006-01-29-SimpleIndirectCall.c
    printargs.c
    TestLoop.c
  )

if (CMAKE_BUILD_TYPE STREQUAL Debug)
  list(REMOVE_ITEM Source fixed_point.cpp)
endif()
endif()

# Enable matrix types extension tests for compilers supporting -fenable-matrix.
check_c_compiler_flag(-fenable-matrix COMPILER_HAS_MATRIX_FLAG)
if (COMPILER_HAS_MATRIX_FLAG)
  set_property(SOURCE matrix-types-spec.cpp PROPERTY COMPILE_FLAGS -fenable-matrix)
else()
  list(REMOVE_ITEM Source matrix-types-spec.cpp)
endif()

llvm_singlesource()

if(ARCH STREQUAL "MOS")
  target_link_libraries(2002-05-02-ArgumentTest PRIVATE printf_flt)
  target_link_libraries(2002-05-02-ManyArguments PRIVATE printf_flt)
  target_link_libraries(2002-10-12-StructureArgs PRIVATE printf_flt)
  target_link_libraries(2002-10-12-StructureArgsSimple PRIVATE printf_flt)
  target_link_libraries(2003-05-07-VarArgs PRIVATE printf_flt)
  target_link_libraries(2003-08-05-CastFPToUint PRIVATE printf_flt)
  target_link_libraries(2003-08-11-VaListArg PRIVATE printf_flt)
  target_link_libraries(2004-02-02-NegativeZero PRIVATE printf_flt)
  target_link_libraries(2006-12-01-float_varg PRIVATE printf_flt)
  target_link_libraries(2007-01-04-KNR-Args PRIVATE printf_flt)
  target_link_libraries(2009-04-16-BitfieldInitialization PRIVATE printf_flt)
  target_link_libraries(2009-12-07-StructReturn PRIVATE printf_flt)
  target_link_libraries(FloatPrecision PRIVATE printf_flt)
  target_link_libraries(StructModifyTest PRIVATE printf_flt)
  target_link_libraries(byval-alignment PRIVATE printf_flt)
  target_link_libraries(printf_float PRIVATE printf_flt)
  if (NOT CMAKE_BUILD_TYPE STREQUAL Debug)
    target_link_libraries(fixed_point PRIVATE printf_flt)
  endif()
else()
  if (COMPILER_HAS_MATRIX_FLAG)
    set_property(TARGET matrix-types-spec PROPERTY LINK_OPTIONS -fenable-matrix)
  endif()
endif()
