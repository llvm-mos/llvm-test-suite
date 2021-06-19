# Modified by LLVM-MOS.

if(NOT ARCH STREQUAL "MOS")
  add_subdirectory(Benchmarks)
endif()
if(NOT TEST_SUITE_BENCHMARKING_ONLY)
  add_subdirectory(UnitTests)
  add_subdirectory(Regression)
endif()

file(COPY lit.local.cfg DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
