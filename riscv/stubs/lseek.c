#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include "stub.h"

off_t _lseek(int fd, off_t ptr, int dir) {
	if (isatty(fd))
		return 0;
	else
		return _stub(EBADF);
}
