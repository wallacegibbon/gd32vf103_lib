#ifndef __FD_H
#define __FD_H

#ifndef FILE_DESCRIPTOR_NUM
#define FILE_DESCRIPTOR_NUM 10
#endif

typedef int (*print_char_fn)(int ch);

extern print_char_fn file_descriptors[FILE_DESCRIPTOR_NUM];

void bind_file_descriptor(int fd, print_char_fn fn);

#endif

