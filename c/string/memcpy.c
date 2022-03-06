#include <string.h>
#include <stdint.h>

void *memcpy(void *dest, const void *src, size_t n) {
	char *tmp = dest;
	const char *s = src;

	while (n--)
		*tmp++ = *s++;

	return dest;
}
