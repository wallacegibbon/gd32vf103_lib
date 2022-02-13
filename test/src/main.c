#include <stdio.h>

char my_variable[] = "hello, world!";
char buf[100];

int main(int argc, const char **argv) {
	for (int i = 0; i < 100; i++)
		buf[i] = my_variable[i % 10];

	return 0;
}
