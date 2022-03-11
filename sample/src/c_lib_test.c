#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This array will be put in the `.data` section and it will be initialized by
// the startup code (in the assembly language file)
char my_variable[] = "hello, world!";

static void misc_c_lib_test() {
	char buf1[] = "hello, this is from buffer 1.";
	char buf2[] = "hello, quick brown fox jumps over the lazy dog.";

	printf("buf1(%p): %s\r\nbuf2(%p): %s\r\n", buf1, buf1, buf2, buf2);

	printf("strcmp(buf1, buf2) = %d\r\n", strcmp(buf1, buf2));
	printf("strcmp(buf2, buf1) = %d\r\n", strcmp(buf2, buf1));

	printf("memcmp(buf1, buf2, 3) = %d\r\n", memcmp(buf1, buf2, 3));
	printf("memcmp(buf2, buf1, 3) = %d\r\n", memcmp(buf2, buf1, 3));

	printf("memcmp(buf1, buf2, 10) = %d\r\n", memcmp(buf1, buf2, 10));
	printf("memcmp(buf2, buf1, 10) = %d\r\n", memcmp(buf2, buf1, 10));

	printf("strlen(buf1) = %d\r\n", strlen(buf1));

	printf("memcpy(buf1, buf2, 10): %p\r\n", memcpy(buf1, buf2, 10));
	printf("buf1(%p): %s\r\nbuf2(%p): %s\r\n", buf1, buf1, buf2, buf2);

	printf("memset(buf1, 'a', 10): %p\r\n", memset(buf1, 'a', 10));
	printf("buf1(%p): %s\r\nbuf2(%p): %s\r\n", buf1, buf1, buf2, buf2);

	printf("\r\n");
}

static void printf_float_test() {
	printf("floating point number print:\r\n");

	printf("\t\t pi = |%010.3f|,  e = |%10.3f|\r\n", 3.1415926, 2.718281);

	printf("negative\t-pi = |%010.3f|, -e = |%10.3f|\r\n",
			-3.1415926, -2.718281);

	printf("'-'\t\t-pi = |%-010.3f|, -e = |%-10.3f|\r\n",
			-3.1415926, 2.718281);

	printf("partial\t\t pi = |%.4f|, e = |%10f|\r\n",
			3.1415926, 2.718281);

	printf("no precision\t-pi = |%10f|,  e = |%10f|\r\n",
			-3.1415926, 2.718281);

	printf("short & noarg\t pi = |%3f|, e = |%f|\r\n",
			3.1415926, 2.718281);

	printf("\r\n");
}

static void printf_width_test() {
	printf("too big width test:\r\n|%0100f|\r\n", -3.1415926);
	printf("too big width test:\r\n|%100f|\r\n", -3.1415926);
	printf("too big width test:\r\n|%-0100f|\r\n", -3.1415926);

	printf("star test: |%*.*f|\r\n", 10, 3, 3.1415926);
	printf("star test: |%0*.*f|\r\n", 10, 3, 3.1415926);
	printf("star test: |%-0*.*f|\r\n", 10, 3, 3.1415926);

	printf("string pad test: |%10.3s|\r\n", "hello");
	printf("string pad test: |%010.3s|\r\n", "hello");
	printf("string pad test: |%-10.3s|\r\n", "hello");
	printf("string pad test: |%-10s|\r\n", "hello");
	printf("string pad test: |%-.3s|\r\n", "hello");
	printf("string pad test: |%-.8s|\r\n", "hello");
	printf("string pad test: |%-10.8s|\r\n", "hello");
	printf("string pad test: |%10.8s|\r\n", "hello");
	printf("string pad test: |%4s|\r\n", "hello");
}

static void printf_misc_test() {
	int n = printf("this is from the serial port, %s\r\n", my_variable);
	printf("the size of previous printf is %04d(%04x)%c\r\n", n, n, '~');

	// pointer printing test
	printf("the address of a string literal: %p\r\n", "hi");
}

/* fd should be initialized before calling dprintf */
static void dprintf_test() {
	dprintf(1, "dprintf test: |%010.3f|\r\n", 3.1415926);
}

void c_lib_test() {
	misc_c_lib_test();

	printf_float_test();
	printf_width_test();

	printf_misc_test();

	dprintf_test();
}

