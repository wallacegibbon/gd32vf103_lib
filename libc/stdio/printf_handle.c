#include "printf_handle.h"
#include "common_util.h"

#define ABS_NUM_AND_RET_SIGN(num) (((num) < 0) ? (num = -num, 1) : 0)

static inline int print_char_n(struct printf_handle *ph, int ch,
		unsigned short n) {

	while (n--)
		ph->print_char(ch);

	return n;
}

static inline int print_zero_with_width(struct printf_handle *ph, int width,
		char pad_char) {

	if (width > 0)
		return print_char_n(ph, pad_char, width - 1);
	else
		return ph->print_char('0');
}

static inline int print_pad_and_sign(struct printf_handle *ph, int is_minus,
		int rest_space, char pad_char) {

	int cnt = 0;
	if (pad_char == ' ') {
		if (rest_space > 0)
			cnt = print_char_n(ph, pad_char, rest_space - is_minus);
		if (is_minus)
			ph->print_char('-');
	} else {
		if (is_minus)
			ph->print_char('-');
		if (rest_space > 0)
			cnt = print_char_n(ph, pad_char, rest_space - is_minus);
	}
	return cnt;
}

static inline int print_only_sign(struct printf_handle *ph, int is_minus) {
	if (is_minus)
		ph->print_char('-');
}

/*
 * doing 64bit division on 32bit system needs c-lib support. (__udivdi3)
 * using `long` could elimitnate the need for c-lib and do not waste the power.
 *	(long is efficient for both `-ilp32` and `-lp64`)
 */
static int print_int(struct printf_handle *ph, long num, int radix, int width,
		char pad_char) {

	if (num == 0)
		return print_zero_with_width(ph, width, pad_char);

	// 20 decimal characters is enough even for 64bit number
	char buf[20];
	int cnt = 0;

	int is_minus = ABS_NUM_AND_RET_SIGN(num);

	for (; cnt < 20 && num > 0; cnt++, num /= radix)
		buf[cnt] = num_to_char(num % radix);

	int pad_cnt = 0;
	int rest_space = width - cnt;

	// "-0003" & "   -3" & "-3   "
	if (!ph->pad_tail)
		pad_cnt = print_pad_and_sign(ph, is_minus, rest_space,
				pad_char);
	else
		print_only_sign(ph, is_minus);

	for (int i = cnt - 1; i >= 0; i--)
		ph->print_char(buf[i]);

	if (ph->pad_tail && rest_space > 0)
		pad_cnt = print_char_n(ph, ' ', rest_space - is_minus);

	return cnt + is_minus + pad_cnt;
}

/* 4294967296(2**32, 10 based) has 10 characters, so 9999999999 is invalid */
#define MAX_DECIMAL_WIDTH 9

#ifndef PRINTF_MAX_WIDTH
#define PRINTF_MAX_WIDTH 80
#endif

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

static inline int adjust_zero_decimal_width(struct printf_handle *ph,
		int int_width) {

	int extra_width = 0;

	if (ph->total_width > int_width + 1) {
		ph->decimal_width = ph->total_width - int_width - 1;
		if (ph->decimal_width > MAX_DECIMAL_WIDTH) {
			extra_width = ph->decimal_width - MAX_DECIMAL_WIDTH;
			ph->decimal_width = MAX_DECIMAL_WIDTH;
		}
	} else if (ph->total_width > 0) {
		// total_width too small, sacrifice space for decimal part
		ph->decimal_width = 1;
	} else {
		// both width and decimal_width are 0
		ph->decimal_width = 6;
	}

	return extra_width;
}

static inline int calc_extra_width(struct printf_handle *ph, int int_width) {

	int r = ph->total_width - int_width - ph->decimal_width - 1;

	if (r > 0)
		return r;
	else
		return 0;
}

static inline int adjust_decimal_width(struct printf_handle *ph, int int_width) {
	if (ph->decimal_width == 0)
		return adjust_zero_decimal_width(ph, int_width);
	else
		return calc_extra_width(ph, int_width);
}

static int print_float(struct printf_handle *ph, double num) {
	if (num > MAX_LONG_VALUE || num < MIN_LONG_VALUE)
		return ph->print_char('!');

	int is_minus = ABS_NUM_AND_RET_SIGN(num);

	// put the int part and decimal part into 2 long integers.
	long int_part = (long) num;
	int int_width = length_of_num(int_part) + is_minus;

	int extra_width = adjust_decimal_width(ph, int_width);

	long decimal_part =
		(long) ((num - int_part) * pow(10, ph->decimal_width));

	// the pad is in int or decial part depending on the pad_tail
	if (ph->pad_tail)
		ph->decimal_width += extra_width;
	else
		int_width += extra_width;

	// the minus follows the int part.
	if (is_minus)
		int_part = -int_part;

	int cnt = print_int(ph, int_part, 10, int_width, ph->pad_char);
	cnt += ph->print_char('.');
	cnt += print_int(ph, decimal_part, 10, ph->decimal_width, '0');

	return cnt;
}

#endif

static void print_str_with_pad(struct printf_handle *ph, char *str) {
	if (ph->total_width == 0 && ph->decimal_width == 0) {
		for (int i = 0; str[i] != '\0'; i++)
			ph->cnt += ph->print_char(str[i]);
		return;
	}

	if (ph->total_width == 0 && ph->decimal_width > 0) {
		for (int i = 0; i < ph->decimal_width && str[i] != '\0'; i++)
			ph->cnt += ph->print_char(str[i]);
		return;
	}

	// ph->total_width > 0

	int len = 0;
	while (len < ph->total_width && str[len] != '\0')
		len++;

	int pad_width = ph->total_width - len;

	if (ph->decimal_width > 0 && len > ph->decimal_width)
		pad_width = ph->total_width - ph->decimal_width;

	if (!ph->pad_tail)
		ph->cnt += print_char_n(ph, ' ', pad_width);

	int fixed_len = ph->total_width - pad_width;
	for (int i = 0; i < fixed_len; i++)
		ph->print_char(str[i]);

	ph->cnt += fixed_len;

	if (ph->pad_tail)
		ph->cnt += print_char_n(ph, ' ', pad_width);
}

static int printf_handle_reset_flag_arg(struct printf_handle *ph) {
	ph->total_width = 0;
	ph->decimal_width = 0;
	ph->pad_tail = 0;
	ph->pad_char = ' ';
}

static void printf_handle_flag(struct printf_handle *ph) {
	switch (ph->ch) {
	case 'd':
		ph->cnt += print_int(ph, va_arg(ph->ap, int), 10,
				ph->total_width, ph->pad_char);
		printf_handle_reset_flag_arg(ph);
		ph->state = PRINTF_NORMAL;
		break;
	case 'X':
	case 'x':
		ph->cnt += print_int(ph, va_arg(ph->ap, int), 16,
				ph->total_width, ph->pad_char);
		printf_handle_reset_flag_arg(ph);
		ph->state = PRINTF_NORMAL;
		break;
	case 'p':
		ph->cnt += print_int(ph, va_arg(ph->ap, int), 16,
				sizeof(int *) * 2, '0');
		printf_handle_reset_flag_arg(ph);
		ph->state = PRINTF_NORMAL;
		break;
	case 'l':
		ph->state = PRINTF_FLAG_L;
		break;
	case 'f':
#if USE_FLOAT == 1
		ph->cnt += print_float(ph, va_arg(ph->ap, double));
		printf_handle_reset_flag_arg(ph);
#else
		va_arg(ph->ap, double);
		ph->cnt += ph->print_char('?');
#endif
		ph->state = PRINTF_NORMAL;
		break;
	case 's':
		print_str_with_pad(ph, va_arg(ph->ap, char *));
		ph->state = PRINTF_NORMAL;
		break;
	case 'c':
		ph->cnt += ph->print_char(va_arg(ph->ap, int));
		ph->state = PRINTF_NORMAL;
		break;
	case '%':
		ph->cnt += ph->print_char('%');
		ph->state = PRINTF_NORMAL;
		break;
	case '0':
		ph->pad_char = '0';
		ph->state = PRINTF_WIDTH;
		break;
	case '.':
		ph->state = PRINTF_DECIMAL_WIDTH;
		break;
	case '-':
		ph->pad_tail = 1;
		break;
	case '*':
		ph->fmt_idx--;
		ph->state = PRINTF_WIDTH;
		break;
	default:
		if (is_decimal_char(ph->ch)) {
			ph->fmt_idx--;
			ph->state = PRINTF_WIDTH;
		} else {
			va_arg(ph->ap, int);
			ph->cnt += ph->print_char('!');
		}
	}
}

static void printf_handle_flag_l(struct printf_handle *ph) {
	switch (ph->ch) {
	case 'd':
		ph->cnt += print_int(ph, va_arg(ph->ap, long), 10, 0, ' ');
		printf_handle_reset_flag_arg(ph);
		ph->state = PRINTF_NORMAL;
		break;
	default:
		ph->cnt += ph->print_char('!');
		ph->state = PRINTF_NORMAL;
	}
}

static inline void limit_value(int *value, int limit) {
	if (*value > limit)
		*value = limit;
}

static void printf_handle_width(struct printf_handle *ph) {
	if (is_decimal_char(ph->ch)) {
		ph->total_width = ph->total_width * 10 + (ph->ch - '0');
		return;
	}

	if (ph->ch == '.') {
		ph->state = PRINTF_DECIMAL_WIDTH;
	} else if (ph->ch == '*') {
		ph->total_width = va_arg(ph->ap, int);
	} else {
		ph->fmt_idx--;
		ph->state = PRINTF_FLAG;
	}

	limit_value(&ph->total_width, PRINTF_MAX_WIDTH);
}

static void printf_handle_decimal_width(struct printf_handle *ph) {
	if (is_decimal_char(ph->ch)) {
		ph->decimal_width = ph->decimal_width * 10 + (ph->ch - '0');
		return;
	}

	if (ph->ch == '.') {
		// although '.' after '.' is invalid ...
		ph->state = PRINTF_NORMAL;
	} else if (ph->ch == '*') {
		ph->decimal_width = va_arg(ph->ap, int);
		ph->state = PRINTF_FLAG;
	} else {
		ph->fmt_idx--;
		ph->state = PRINTF_FLAG;
	}

	limit_value(&ph->decimal_width, MAX_DECIMAL_WIDTH);
}

static void printf_handle_normal(struct printf_handle *ph) {
	switch (ph->ch) {
	case '%':
		ph->state = PRINTF_FLAG;
		break;
	default:
		ph->cnt += ph->print_char(ph->ch);
	}
}

// With Caution: A `va_start(ph_var_name.ap, fmt_p_str);` should also be called.
void printf_handle_init(struct printf_handle *ph, const char *fmt,
		int (*print_char)(int)) {

	ph->state = PRINTF_NORMAL;
	ph->fmt = fmt;
	ph->fmt_idx = 0;
	ph->total_width = 0;
	ph->decimal_width = 0;
	ph->cnt = 0;
	ph->pad_tail = 0;
	ph->pad_char = ' ';
	ph->print_char = print_char;
}

static int printf_handle_next_char(struct printf_handle *ph) {
	ph->ch = ph->fmt[ph->fmt_idx++];
	return ph->ch;
}

static void printf_handle_entry(struct printf_handle *ph) {
	switch (ph->state) {
	case PRINTF_FLAG:
		printf_handle_flag(ph);
		break;
	case PRINTF_FLAG_L:
		printf_handle_flag_l(ph);
		break;
	case PRINTF_WIDTH:
		printf_handle_width(ph);
		break;
	case PRINTF_DECIMAL_WIDTH:
		printf_handle_decimal_width(ph);
		break;
	case PRINTF_NORMAL:
		printf_handle_normal(ph);
		break;
	}
}

void printf_handle_loop(struct printf_handle *ph) {
	while (printf_handle_next_char(ph))
		printf_handle_entry(ph);
}

