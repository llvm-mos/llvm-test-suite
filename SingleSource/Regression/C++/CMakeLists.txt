if(NOT ARCH STREQUAL "MOS")
  add_subdirectory(EH)
endif()

if(ARCH MATCHES "MOS")
  file(GLOB Source RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

  list(REMOVE_ITEM Source
    # Temporarily disabled tests.
    # ---------------------------

    # Requires RTTI.
    BuiltinTypeInfo.cpp
  )
endif()

llvm_singlesource(PREFIX "Regression-C++-")
