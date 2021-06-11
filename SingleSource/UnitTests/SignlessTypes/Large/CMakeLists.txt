set(RUN_OPTIONS 31415926)
set(HASH_PROGRAM_OUTPUT 1)

if(ARCH STREQUAL "MOS")
  file(GLOB Source RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c *.cpp)
  list(REMOVE_ITEM Source
    # Temporarily disabled tests.
    # ---------------------------

    # Requires argc/argv
    cast.cpp
  )
endif()
llvm_singlesource()
