add_subdirectory(C)

include(CheckCXXSourceCompiles)
check_cxx_source_compiles("int main() {
  try { throw 1; } catch (int) { return 0; }
}" SETJMP_LONGJMP_HAS_CXX_EXCEPTIONS)
if(SETJMP_LONGJMP_HAS_CXX_EXCEPTIONS)
  add_subdirectory(C++)
endif()
