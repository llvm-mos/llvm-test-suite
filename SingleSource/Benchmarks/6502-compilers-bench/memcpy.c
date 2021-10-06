/*
 * A simplified memcpy, with `n` being on one byte and without return value.
 */
#include <stdint.h>

void memcpy_8bits_c_style(uint8_t* dest, uint8_t* src, uint8_t n) {
	uint8_t i;

	for (i = 0; i < n; ++i) {
		dest[i] = src[i];
	}
}

uint8_t dst[200];
uint8_t src[200];

volatile char v;

int main(void) {
	src[0] = 42;
	memcpy_8bits_c_style(dst, src, 200);
	v = dst[0];
	return 0;
}

