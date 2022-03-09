#include <stdio.h>
#include <stdarg.h>
#include "printf_handle.h"
#include "../custom/fd.h"

int dprintf(int fd, const char *fmt, ...) {
	struct printf_handle handle;

	if (!file_descriptors[fd])
		return 0;

	printf_handle_init(&handle, fmt, file_descriptors[fd]);

	va_start(handle.ap, fmt);

	printf_handle_loop(&handle);

	va_end(handle.ap);

	return handle.cnt;
}

