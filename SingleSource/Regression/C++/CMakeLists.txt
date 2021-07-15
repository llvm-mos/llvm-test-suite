# Modified by Daniel Thornburgh (mysterymath@gmail.com).

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

    # Requires RTTI.
    BuiltinTypeInfo.cpp

    # Requires global constructors/destructors.
    global_ctor.cpp

    # Requires vtables.
    pointer_method2.cpp
  )
endif()

llvm_singlesource(PREFIX "Regression-C++-")
