cmake_minimum_required(VERSION 3.13)

# CMake should consider MOS as an embedded platform with no OS assumptions.
set(CMAKE_SYSTEM_NAME Generic)

# Find mos compilers if not already set.
include(${CMAKE_CURRENT_LIST_DIR}/find-mos-compiler.cmake)
find_mos_compiler(CMAKE_C_COMPILER mos-sim-clang)
find_mos_compiler(CMAKE_CXX_COMPILER mos-sim-clang++)
find_mos_compiler(CMAKE_ASM_COMPILER mos-sim-clang)
if(NOT CMAKE_C_COMPILER OR NOT CMAKE_CXX_COMPILER OR NOT CMAKE_ASM_COMPILER)
  message(FATAL_ERROR "Did not find LLVM-MOS compiler.")
endif()
