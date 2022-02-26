#ifndef __COMMON_UTIL_H
#define __COMMON_UTIL_H

static inline int is_decimal_char(char ch) {
	return ch >= '0' && ch <= '9';
}

static inline char num_to_char(int num) {
	if (num <= 9)
		return num + '0';
	else if (num > 9 && num < 16)
		return num - 10 + 'A';
	else
		return '*';
}

static inline int putchar_n(int ch, int n) {
	int r = n;
	if (n <= 0)
		return 0;

	while (n--)
		putchar(ch);

	return r;
}

#endif

