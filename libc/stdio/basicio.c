#include <stdio.h>

int puts(const char *s) {
	int cnt = 0;
	for (; *s != '\0'; cnt++, s++)
		putchar(*s);
	return cnt;
}

__attribute__((weak))
int putchar(int ch) {
	return ch;
}

