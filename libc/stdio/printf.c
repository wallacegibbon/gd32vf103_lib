#include <stdio.h>
#include <stdarg.h>

enum printf_state {
	PRINTF_NORMAL,
	PRINTF_FLAG,
	PRINTF_FLAG_L,
	PRINTF_MODIFIER,
	PRINTF_WIDTH,
	PRINTF_PRECISION
};

static inline char num_to_char(int num) {
	if (num <= 9)
		return num + '0';
	else if (num > 9 && num < 16)
		return num - 10 + 'A';
	else
		return '*';
}

static int format_int(int num, int radix) {
	// int64 may have 20 decimal characters
	char buf[20];
	int cnt = 0;

	for (; cnt < 20 && num > 0; cnt++, num /= radix)
		buf[cnt] = num_to_char(num % radix);

	for (int i = cnt - 1; i >= 0; i--)
		putchar(buf[i]);

	return cnt;
}

static int format_float(double num) {
	return 0;
}

static enum printf_state handle_char(char ch, va_list ap,
		enum printf_state state, int *cnt) {

	switch (state) {
	case PRINTF_NORMAL:
		if (ch == '%')
			state = PRINTF_FLAG;
		else if (ch == '\\')
			state = PRINTF_MODIFIER;
		else
			*cnt += putchar(ch);
		break;
	case PRINTF_FLAG:
		switch (ch) {
		case 'X':
		case 'x':
			*cnt += format_int(va_arg(ap, int), 16);
			break;
		case 'd':
			*cnt += format_int(va_arg(ap, int), 10);
			break;
		case 'l':
			state = PRINTF_FLAG_L;
			break;
		case 'f':
			//*cnt += format_float(va_arg(ap, float));
			*cnt += format_float(va_arg(ap, double));
			break;
		case 's':
			*cnt += puts(va_arg(ap, char *));
			break;
		}
		break;
	case PRINTF_FLAG_L:
		if (ch == 'f')
			*cnt += format_float(va_arg(ap, double));
		break;
	case PRINTF_MODIFIER:
		switch (ch) {
		case 'n':
			*cnt += putchar('\n');
			break;
		case 'r':
			*cnt += putchar('\r');
			break;
		case 't':
			*cnt += putchar('\t');
			break;
		case 'v':
			*cnt += putchar('\v');
			break;
		case 'b':
			*cnt += putchar('\b');
			break;
		default:
			*cnt += putchar(ch);
		}
		break;
	}

	return state;
}

int printf(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	const char *p = fmt;
	char ch;
	enum printf_state state = PRINTF_NORMAL;
	int cnt = 0;

	while (ch = *p++)
		state = handle_char(ch, ap, state, &cnt);

	va_end(ap);

	return cnt;
}

