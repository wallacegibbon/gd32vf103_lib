#include "fd.h"

print_char_fn file_descriptors[FILE_DESCRIPTOR_NUM];

void bind_file_descriptor(int fd, print_char_fn fn) {
	file_descriptors[fd] = fn;
}

