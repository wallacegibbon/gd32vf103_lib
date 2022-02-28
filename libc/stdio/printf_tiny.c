#include <stdio.h>
#include <stdarg.h>
#include "common_util.h"

#ifdef USE_TINY_PRINTF

/*
 * doing 64bit division on 32bit system needs c-lib support. (__udivdi3)
 * using `long` could elimitnate the need for c-lib and do not waste the power.
 *	(long is efficient for both `-ilp32` and `-lp64`)
 */
static int print_int(long num, int radix) {
	// 20 decimal characters is enough even for 64bit number
	char buf[20];
	int cnt = 0;

	if (num == 0)
		return putchar('0');

	int is_minus = 0;
	if (num < 0) {
		cnt += putchar('-');
		num = -num;
	}

	for (; cnt < 20 && num > 0; cnt++, num /= radix)
		buf[cnt] = num_to_char(num % radix);

	for (int i = cnt - 1; i >= 0; i--)
		putchar(buf[i]);

	return cnt;
}

#if USE_FLOAT == 1

#define MAX_LONG_VALUE ((long) (((unsigned long) -1) / 2))
#define MIN_LONG_VALUE (-MAX_LONG_VALUE - 1)

static int print_float(double num) {

	if (num > MAX_LONG_VALUE || num < MIN_LONG_VALUE)
		return putchar('!');

	int is_minus = 0;
	if (num < 0) {
		is_minus = 1;
		num = -num;
	}

	// put the int part and decimal part into 2 long integers.
	long int_part = (long) num;
	long decimal_part = (long) ((num - int_part) * 10000);

	if (is_minus)
		int_part = -int_part;

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
	PRINTF_FLAG_L
};

struct printf_state {
	enum printf_state_flag state;
	const char *fmt;
	int fmt_idx;
	va_list ap;
	char ch;
	int cnt;
};

void printf_state_init(struct printf_state *st, const char *fmt) {
	st->state = PRINTF_NORMAL;
	st->fmt = fmt;
	st->fmt_idx = 0;
	st->cnt = 0;
}

int printf_state_next_char(struct printf_state *st) {
	st->ch = st->fmt[st->fmt_idx++];
	return st->ch;
}

void printf_handle_flag(struct printf_state *st) {
	switch (st->ch) {
	case 'd':
		st->cnt += print_int(va_arg(st->ap, int), 10);
		st->state = PRINTF_NORMAL;
		break;
	case 'X':
	case 'x':
	case 'p':
		st->cnt += print_int(va_arg(st->ap, int), 16);
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
	case 'c':
		st->cnt += putchar(va_arg(st->ap, int));
		st->state = PRINTF_NORMAL;
		break;
	case '%':
		st->cnt += putchar('%');
		st->state = PRINTF_NORMAL;
		break;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '.':
	case '-':
	case '*':
		// ignore the width control
		break;
	default:
		va_arg(st->ap, int);
		st->cnt += putchar('!');
	}
}

void printf_handle_flag_l(struct printf_state *st) {
	switch (st->ch) {
	case 'd':
		st->cnt += print_int(va_arg(st->ap, long), 10);
		st->state = PRINTF_NORMAL;
		break;
	default:
		st->cnt += putchar('!');
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
	struct printf_state state;

	printf_state_init(&state, fmt);

	va_start(state.ap, fmt);

	while (printf_state_next_char(&state))
		printf_sub(&state);

	va_end(state.ap);

	return state.cnt;
}

#endif

