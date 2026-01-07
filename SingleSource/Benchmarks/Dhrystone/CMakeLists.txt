if (ARCH STREQUAL "MOS")
  file(GLOB Source RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c)
  list(REMOVE_ITEM Source
    # Temporarily disabled tests.
    # ---------------------------

    # Requires floating point.
    fldry.c
  )
endif()

list(APPEND LDFLAGS -lm)
list(APPEND CFLAGS -Wno-implicit-int)
llvm_singlesource()
if (NOT ARCH STREQUAL "MOS")
  set_property(TARGET dry fldry PROPERTY C_STANDARD 17)
endif()
