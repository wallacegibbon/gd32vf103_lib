#include <stdio.h>
#include <stdarg.h>
#include "printf_handle.h"

#ifndef USE_TINY_PRINTF

int printf(const char *fmt, ...) {
	struct printf_handle handle;

	printf_handle_init(&handle, fmt, putchar);

	va_start(handle.ap, fmt);

	printf_handle_loop(&handle);

	va_end(handle.ap);

	return handle.cnt;
}

#endif

