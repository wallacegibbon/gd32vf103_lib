#include <stdio.h>
#include <stdarg.h>

enum printf_state {
	PRINTF_NORMAL,
	PRINTF_FLAG,
	PRINTF_FLAG_L,
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

static int print_int(int num, int radix) {
	// 20 decimal characters is enough even for 64bit number
	char buf[20];
	int cnt = 0;

	for (; cnt < 20 && num > 0; cnt++, num /= radix)
		buf[cnt] = num_to_char(num % radix);

	for (int i = cnt - 1; i >= 0; i--)
		putchar(buf[i]);

	return cnt;
}

#if USE_FLOAT == 1
static int print_float(double num) {
	if (num > (unsigned int) -1)
		return puts("<too big>");

	int int_part = (int) num;
	int decimal_part = (int) ((num - int_part) * 10000);
	int cnt = print_int(int_part, 10);
	cnt += putchar('.');
	cnt += print_int(decimal_part, 10);

	return cnt;
}
#endif

static enum printf_state printf_sub(char ch, enum printf_state state,
		int *cnt, va_list *ap) {
#if USE_FLOAT != 1
	int ignore;
#endif
	switch (state) {
	case PRINTF_FLAG:
		switch (ch) {
		case 'X':
		case 'x':
			*cnt += print_int(va_arg(*ap, int), 16);
			state = PRINTF_NORMAL;
			break;
		case 'd':
			*cnt += print_int(va_arg(*ap, int), 10);
			state = PRINTF_NORMAL;
			break;
		case 'l':
			state = PRINTF_FLAG_L;
			break;
		case 'f':
#if USE_FLOAT == 1
			//*cnt += print_float(va_arg(*ap, float));
			*cnt += print_float(va_arg(*ap, double));
#else
			ignore = va_arg(*ap, double);
			*cnt += putchar('*');
#endif
			state = PRINTF_NORMAL;
			break;
		case 's':
			*cnt += puts(va_arg(*ap, char *));
			state = PRINTF_NORMAL;
			break;
		case '%':
			*cnt += putchar('%');
			state = PRINTF_NORMAL;
			break;
		}
		break;
	case PRINTF_FLAG_L:
		if (ch == 'f') {
#if USE_FLOAT == 1
			*cnt += print_float(va_arg(*ap, double));
#else
			ignore = va_arg(*ap, double);
			*cnt += putchar('*');
#endif
		} else {
			*cnt += putchar('*');
		}
		state = PRINTF_NORMAL;
		break;
	case PRINTF_NORMAL:
		if (ch == '%')
			state = PRINTF_FLAG;
		else
			*cnt += putchar(ch);
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
		state = printf_sub(ch, state, &cnt, &ap);

	va_end(ap);
	return cnt;
}

