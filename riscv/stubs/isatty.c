#include <unistd.h>

int _isatty(int fd) {
	if (fd == STDOUT_FILENO || fd == STDERR_FILENO)
		return 1;
	else
		return 0;
}
