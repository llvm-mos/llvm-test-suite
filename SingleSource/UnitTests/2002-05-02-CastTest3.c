// Modified by Daniel Thornburgh (mysterymath@gmail.com).

int printf(const char *, ...);

int main(void) {
  short s1 = -769; /* 0xf7ff = -769 */

  unsigned short us2 = (unsigned short) s1;     /* 0xf7ff = 64767 */
  
  printf("s1   = %d\n",   s1);
  printf("us2  = %u\n",   us2);
  return 0;
}
