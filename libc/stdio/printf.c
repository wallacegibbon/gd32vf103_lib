#include <stdio.h>
#include <stdarg.h>

static inline char num_to_char(int num) {
	if (num <= 9)
		return num + '0';
	else if (num > 9 && num < 16)
		return num - 10 + 'A';
	else
		return '*';
}

/*
 * doing 64bit division on 32bit system needs c-lib support. (__udivdi3)
 * using `long` could elimitnate the need for c-lib and do not waste the power.
 *	(long is efficient for both `-ilp32` and `-lp64`)
 */
static int print_int(long num, int radix) {
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
	if (num > (unsigned long) -1)
		return putchar('*');

	long int_part = (long) num;
	long decimal_part = (long) ((num - int_part) * 10000);
	int cnt = print_int(int_part, 10);
	cnt += putchar('.');
	cnt += print_int(decimal_part, 10);

	return cnt;
}
#endif

/* the printf function is basicly a state machine */
enum printf_state_flag {
	PRINTF_NORMAL,
	PRINTF_FLAG,
	PRINTF_FLAG_L,
	PRINTF_WIDTH,
	PRINTF_PRECISION
};

struct printf_state {
	enum printf_state_flag state;
	va_list ap;
	const char *fmt;
	int cnt;
	char ch;
};

int printf_state_nextchar(struct printf_state *st) {
	st->ch = *st->fmt++;
	return st->ch;
}

void printf_handle_flag(struct printf_state *st) {
	switch (st->ch) {
	case 'X':
	case 'x':
		st->cnt += print_int(va_arg(st->ap, int), 16);
		st->state = PRINTF_NORMAL;
		break;
	case 'd':
		st->cnt += print_int(va_arg(st->ap, int), 10);
		st->state = PRINTF_NORMAL;
		break;
	case 'l':
		st->state = PRINTF_FLAG_L;
		break;
	case 'f':
#if USE_FLOAT == 1
		st->cnt += print_float(va_arg(st->ap, double));
#else
		va_arg(st->ap, double);
		st->cnt += putchar('?');
#endif
		st->state = PRINTF_NORMAL;
		break;
	case 's':
		st->cnt += puts(va_arg(st->ap, char *));
		st->state = PRINTF_NORMAL;
		break;
	case '%':
		st->cnt += putchar('%');
		st->state = PRINTF_NORMAL;
		break;
	default:
		va_arg(st->ap, int);
		st->cnt += putchar('*');
	}
}

void printf_handle_flag_l(struct printf_state *st) {
	switch (st->ch) {
	case 'd':
		st->cnt += print_int(va_arg(st->ap, long), 10);
		st->state = PRINTF_NORMAL;
		break;
	default:
		st->cnt += putchar('*');
		st->state = PRINTF_NORMAL;
	}
}

void printf_handle_normal(struct printf_state *st) {
	switch (st->ch) {
	case '%':
		st->state = PRINTF_FLAG;
		break;
	default:
		st->cnt += putchar(st->ch);
	}
}

void printf_sub(struct printf_state *st) {
	switch (st->state) {
	case PRINTF_FLAG:
		printf_handle_flag(st);
		break;
	case PRINTF_FLAG_L:
		printf_handle_flag_l(st);
		break;
	case PRINTF_NORMAL:
		printf_handle_normal(st);
		break;
	}
}

int printf(const char *fmt, ...) {
	struct printf_state state = {PRINTF_NORMAL, NULL, fmt, 0, '@'};
	va_start(state.ap, fmt);

	while (printf_state_nextchar(&state))
		printf_sub(&state);

	va_end(state.ap);

	return state.cnt;
}

