if(NOT ARCH STREQUAL "MOS")
  add_subdirectory(EH)
endif()

llvm_singlesource(PREFIX "Regression-C++-")
