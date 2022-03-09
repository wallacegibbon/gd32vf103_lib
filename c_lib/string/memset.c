#include <string.h>
#include <stdint.h>

void *memset(void *s, int c, size_t n) {
	char *s1 = s;
	while (n--)
		*s1++ = c;

	return s;
}
