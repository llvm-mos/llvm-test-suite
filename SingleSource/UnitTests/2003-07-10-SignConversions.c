/*
 * Test basic conversions between short signed and unsigned values
 * with no function calls and no arithmetic.
 */

// Modified by LLVM-MOS.

#include <stdio.h>

unsigned char getUC() { return 0x80; }

signed   char getSC() { return 0x80; }

int main()
{
  signed   char  SC80 = getSC();
  unsigned char  UC80 = getUC();

  /* source is smaller than dest: both decide */
  unsigned short us  = (unsigned short) SC80;     /* sign-ext then zero-ext */
  unsigned short us2 = (unsigned short) UC80;     /* zero-ext only: NOP! */
           short  s  = (         short) SC80;     /* sign-ext */
           short  s2 = (         short) UC80;     /* zero-extend only : NOP! */
  printf("%ld %ld --> unsigned: us = %ld, us2 = %ld\n", (long)SC80, (long)UC80, (long)us, (long)us2);
  printf("%ld %ld -->   signed:  s = %ld,  s2 = %ld\n", (long)SC80, (long)UC80,  (long)s,  (long)s2);

  /* source is same size or larger than dest: dest decides */
  unsigned char  uc  = (unsigned char ) SC80;     /* zero-ext */
  unsigned char  uc2 = (unsigned char ) UC80;     /* NOP */
  signed   char  sc  = (signed   char ) SC80;     /* NOP */
  signed   char  sc2 = (signed   char ) UC80;     /* sign-extend */
  printf("%ld %ld --> unsigned: uc = %ld, uc2 = %ld\n", (long)SC80, (long)UC80, (long)uc, (long)uc2);
  printf("%ld %ld -->   signed: sc = %ld, sc2 = %ld\n", (long)SC80, (long)UC80, (long)sc, (long)sc2);
  return 0;
}
