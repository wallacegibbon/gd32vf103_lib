#include <string.h>
#include <stdint.h>

int memcmp(const void *s1, const void *s2, size_t n) {
	const unsigned char *su1 = s1, *su2 = s2;
	int res = 0;

	while (n--)
		if ((res = *su1++ - *su2++) != 0)
			break;

	return res;
}
