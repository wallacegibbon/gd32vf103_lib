#include <stdio.h>
#include <stdarg.h>
#include "common_util.h"

/*
 * doing 64bit division on 32bit system needs c-lib support. (__udivdi3)
 * using `long` could elimitnate the need for c-lib and do not waste the power.
 *	(long is efficient for both `-ilp32` and `-lp64`)
 */
static int print_int(long num, int radix, int width,
		char fill_char, int fill_tail) {

	// 20 decimal characters is enough even for 64bit number
	if (num == 0) {
		if (width > 0)
			return putchar_n(fill_char, width - 1);
		else
			return putchar('0');
	}

	int is_minus = 0;
	if (num < 0) {
		is_minus = 1;
		num = -num;
	}

	char buf[20];
	int cnt = 0;

	for (; cnt < 20 && num > 0; cnt++, num /= radix)
		buf[cnt] = num_to_char(num % radix);

	int fill_cnt = 0;
	int num_to_fill = width - cnt - is_minus;

	// "-0003" & "   -3" & "-3   "
	if (!fill_tail) {
		if (fill_char == ' ') {
			if (width > cnt)
				fill_cnt = putchar_n(fill_char, num_to_fill);
			if (is_minus)
				putchar('-');
		} else {
			if (is_minus)
				putchar('-');
			if (width > cnt)
				fill_cnt = putchar_n(fill_char, num_to_fill);
		}
	} else {
		if (is_minus)
			putchar('-');
	}

	for (int i = cnt - 1; i >= 0; i--)
		putchar(buf[i]);

	if (fill_tail && width > cnt)
		fill_cnt = putchar_n(' ', num_to_fill);

	return cnt + is_minus + fill_cnt;
}

/* 4294967296(2**32, 10 based) has 10 characters, so 9999999999 is invalid */
#define MAX_DECIMAL_WIDTH 9
#define MAX_TOTAL_WIDTH 80

#if USE_FLOAT == 1

static inline long pow(long x, long y) {
	int r = 1;
	while (y--)
		r *= x;
	return r;
}

static inline int length_of_num(long num) {
	int r = 1;
	while (num /= 10)
		r++;
	return r;
}

#define MAX_LONG_VALUE ((long) (((unsigned long) -1) / 2))
#define MIN_LONG_VALUE (-MAX_LONG_VALUE - 1)

static int print_float(double num, int width, int decimal_width,
		char fill_char, int fill_tail) {

	if (num > MAX_LONG_VALUE || num < MIN_LONG_VALUE)
		return putchar('*');

	int is_minus = 0;
	if (num < 0) {
		is_minus = 1;
		num = -num;
	}

	// put the int part and decimal part into 2 long integers.
	long int_part = (long) num;
	int int_width = length_of_num(int_part) + is_minus;

	int extra_width = 0;

	// adjust int_width and decimal_width
	if (decimal_width == 0) {
		if (width > int_width + 1) {
			decimal_width = width - int_width - 1;
			if (decimal_width > MAX_DECIMAL_WIDTH) {
				extra_width = decimal_width - MAX_DECIMAL_WIDTH;
				decimal_width = MAX_DECIMAL_WIDTH;
			}
		} else if (width > 0) {
			decimal_width = 1;
		} else {
			// both width and decimal_width are 0
			decimal_width = 6;
		}
	} else {
		if (width > int_width + decimal_width + 1)
			extra_width = width - int_width - decimal_width - 1;
	}

	long decimal_part =
		(long) ((num - int_part) * pow(10, decimal_width));

	// the pad is in int or decial part depending on the fill_tail
	if (extra_width) {
		if (fill_tail)
			decimal_width += extra_width;
		else
			int_width += extra_width;
	}

	// the minus follows the int part.
	if (is_minus)
		int_part = -int_part;

	int cnt = print_int(int_part, 10, int_width, fill_char, 0);
	cnt += putchar('.');
	cnt += print_int(decimal_part, 10, decimal_width, '0', fill_tail);

	return cnt;
}
#endif

/* the printf function is basicly a state machine */
enum printf_state_flag {
	PRINTF_NORMAL,
	PRINTF_FLAG,
	PRINTF_FLAG_L,
	PRINTF_WIDTH,
	PRINTF_DECIMAL_WIDTH
};

struct printf_state {
	enum printf_state_flag state;
	const char *fmt;
	int fmt_idx;
	va_list ap;
	int total_width, decimal_width;
	int fill_tail;
	char fill_char;
	char ch;
	int cnt;
};

void printf_state_init(struct printf_state *st, const char *fmt) {
	st->state = PRINTF_NORMAL;
	st->fmt = fmt;
	st->fmt_idx = 0;
	st->total_width = 0;
	st->decimal_width = 0;
	st->cnt = 0;
	st->fill_tail = 0;
	st->fill_char = ' ';
}

int printf_state_next_char(struct printf_state *st) {
	st->ch = st->fmt[st->fmt_idx++];
	return st->ch;
}

int printf_state_reset_flag_arg(struct printf_state *st) {
	st->total_width = 0;
	st->decimal_width = 0;
	st->fill_tail = 0;
	st->fill_char = ' ';
}

void printf_handle_flag(struct printf_state *st) {
	switch (st->ch) {
	case 'd':
		st->cnt += print_int(va_arg(st->ap, int), 10, st->total_width,
				st->fill_char, st->fill_tail);
		printf_state_reset_flag_arg(st);
		st->state = PRINTF_NORMAL;
		break;
	case 'X':
	case 'x':
		st->cnt += print_int(va_arg(st->ap, int), 16, st->total_width,
				st->fill_char, st->fill_tail);
		printf_state_reset_flag_arg(st);
		st->state = PRINTF_NORMAL;
		break;
	case 'p':
		st->cnt += print_int(va_arg(st->ap, int), 16,
				sizeof(int *) * 2, '0', st->fill_tail);
		printf_state_reset_flag_arg(st);
		st->state = PRINTF_NORMAL;
		break;
	case 'l':
		st->state = PRINTF_FLAG_L;
		break;
	case 'f':
#if USE_FLOAT == 1
		st->cnt += print_float(va_arg(st->ap, double),
				st->total_width, st->decimal_width,
				st->fill_char, st->fill_tail);
		printf_state_reset_flag_arg(st);
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
		st->fill_char = '0';
		st->state = PRINTF_WIDTH;
		break;
	case '.':
		st->state = PRINTF_DECIMAL_WIDTH;
		break;
	case '-':
		st->fill_tail = 1;
		break;
	default:
		if (is_decimal_char(st->ch)) {
			st->fmt_idx--;
			st->state = PRINTF_WIDTH;
		} else {
			va_arg(st->ap, int);
			st->cnt += putchar('*');
		}
	}
}

void printf_handle_flag_l(struct printf_state *st) {
	switch (st->ch) {
	case 'd':
		st->cnt += print_int(va_arg(st->ap, long), 10, 0, ' ',
				st->fill_tail);
		printf_state_reset_flag_arg(st);
		st->state = PRINTF_NORMAL;
		break;
	default:
		st->cnt += putchar('*');
		st->state = PRINTF_NORMAL;
	}
}

static inline void limit_value(int *value, int limit) {
	if (*value > limit)
		*value = limit;
}

void printf_handle_width(struct printf_state *st) {
	if (is_decimal_char(st->ch)) {
		st->total_width = st->total_width * 10 + (st->ch - '0');
		return;
	}

	if (st->ch == '.') {
		st->state = PRINTF_DECIMAL_WIDTH;
	} else {
		st->fmt_idx--;
		st->state = PRINTF_FLAG;
	}

	limit_value(&st->total_width, MAX_TOTAL_WIDTH);
}

void printf_handle_decimal_width(struct printf_state *st) {
	if (is_decimal_char(st->ch)) {
		st->decimal_width = st->decimal_width * 10 + (st->ch - '0');
		return;
	}

	if (st->ch == '.') {
		// although '.' after '.' is invalid ...
		st->state = PRINTF_NORMAL;
	} else {
		st->fmt_idx--;
		st->state = PRINTF_FLAG;
	}

	limit_value(&st->decimal_width, MAX_DECIMAL_WIDTH);
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
	case PRINTF_WIDTH:
		printf_handle_width(st);
		break;
	case PRINTF_DECIMAL_WIDTH:
		printf_handle_decimal_width(st);
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

