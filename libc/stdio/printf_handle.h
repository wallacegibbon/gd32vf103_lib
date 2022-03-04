#ifndef __PRINTF_HANDLE_H
#define __PRINTF_HANDLE_H

#include <stdarg.h>

/* the printf function is basicly a state machine */
enum printf_handle_flag {
	PRINTF_NORMAL,
	PRINTF_FLAG,
	PRINTF_FLAG_L,
	PRINTF_WIDTH,
	PRINTF_PRECISION_WIDTH
};

struct printf_handle {
	enum printf_handle_flag state;
	const char *fmt;
	int fmt_idx;
	va_list ap;
	int total_width, precision_width;
	int pad_tail;
	char pad_char;
	char ch;
	int cnt;
	int (*print_char)(int ch);
};

void printf_handle_init(struct printf_handle *ph, const char *fmt,
		int (*print_char)(int));

void printf_handle_loop(struct printf_handle *ph);

#endif

