# Modified by LLVM-MOS.

add_subdirectory(Large)

set(RUN_OPTIONS 31415926)

if(ARCH STREQUAL "MOS")
  file(GLOB Source RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c *.cpp)
  list(REMOVE_ITEM Source
    # Temporarily disabled tests.
    # ---------------------------

    # Requires argc/argv
    ccc.c

    # Requires rand
    rem.c

    # Requires argc/argv
    cast-bug.c
  )
endif()

llvm_singlesource()
