cmake_minimum_required(VERSION 3.13)

# CMake should consider MOS as an embedded platform with no OS assumptions.
set(CMAKE_SYSTEM_NAME Generic)

# Find mos compilers if not already set.
if(WIN32)
  set(extension ".exe")
endif()
set(CMAKE_C_COMPILER ${LLVM_MOS}/bin/mos-sim-clang${extension} CACHE STRING "")
set(CMAKE_CXX_COMPILER ${LLVM_MOS}/bin/mos-sim-clang++${extension} CACHE STRING "")
set(CMAKE_ASM_COMPILER ${LLVM_MOS}/bin/mos-sim-clang${extension} CACHE STRING "")
