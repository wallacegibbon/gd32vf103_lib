#include <stdint.h>
#include <unistd.h>

void write_hex(int fd, unsigned long int hex) {
	char towrite;
	write(fd, "0x", 2);

	for (int i = sizeof(unsigned long int) * 2; i > 0; i--) {
		int j = i - 1;
		uint8_t digit = ((hex & (0xF << (j * 4))) >> (j * 4));
		towrite = digit < 0xA ? ('0' + digit) : ('A' + (digit - 0xA));
		write(fd, &towrite, 1);
	}
}
