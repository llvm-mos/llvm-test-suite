add_subdirectory(Large)

set(RUN_OPTIONS 31415926)

if(ARCH STREQUAL "MOS")
  file(GLOB Source RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c *.cpp)
  list(REMOVE_ITEM Source
    # Permanently disabled tests.
    # ---------------------------

    # Requires 32-bit int.
    div.c
    shr.c
  )
endif()

llvm_singlesource()
