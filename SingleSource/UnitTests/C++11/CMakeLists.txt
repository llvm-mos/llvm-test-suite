# Modified by Daniel Thornburgh (mysterymath@gmail.com).

list(APPEND CXXFLAGS -std=c++11 -pthread)
list(APPEND LDFLAGS -pthread)

file(GLOB Source RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c *.cpp)
if(${CMAKE_EXE_LINKER_FLAGS} MATCHES "-static")
	list(APPEND LDFLAGS -Wl,--whole-archive -lpthread -Wl,--no-whole-archive)
endif()
if(ARCH STREQUAL "MOS")
  file(GLOB Source RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

  list(REMOVE_ITEM Source
    # Permanently disabled tests.
    # ---------------------------

    # Requires std::thread.
    stdthreadbug.cpp
  )
endif()

llvm_singlesource()
