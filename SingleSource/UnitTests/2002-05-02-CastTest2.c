// Modified by LLVM-MOS.

#include <stdlib.h>
#include <stdio.h>

int
main(void) {
  short s1 = -769; /* 0xf7ff = -769 */

  unsigned short us2 = (unsigned short) s1;     /* 0xf7ff = 64767 */
  printf("us2  = %u\n",   us2);
  return 0;
}
