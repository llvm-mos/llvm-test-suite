
if(NOT ARCH STREQUAL "MOS")
  add_subdirectory(EH)
endif()

if(ARCH STREQUAL "MOS")
  file(GLOB Source RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

  list(REMOVE_ITEM Source
    # Permanently disabled tests.
    # ---------------------------

    # Requires iostreams.
    ofstream_ctor.cpp

    # Temporarily disabled tests.
    # ---------------------------

    # Requires cstdio
    BuiltinTypeInfo.cpp
  )
endif()

llvm_singlesource(PREFIX "Regression-C++-")
