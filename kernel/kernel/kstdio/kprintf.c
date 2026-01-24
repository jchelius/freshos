#include <kernel/kstdio.h>

#include <limits.h>
#include <stdarg.h>

#include <kernel/tty.h>
#include <kernel/strutil.h>
#include <kernel/vga.h>

static char *itoa(int value, char *str, int base) {
    char *rc;
    char *ptr;
    char *low;
    // Check for supported base.
    if (base < 2 || base > 36) {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    // Set '-' for negative decimals.
    if (value < 0 && base == 10) {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while (value);
    // Terminating the strutil.
    *ptr-- = '\0';
    // Invert the numbers.
    while (low < ptr) {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

static inline int is_whitespace(char ch) {
	return ch == ' ' || 
		ch == '\t' || 
		ch == '\n' ||
		ch == '\v' ||
		ch == '\f' ||
		ch == '\r';
}

static inline int is_digit(char ch) {
	return ch >= '0' && ch <= '9';
}

static int atoi(const char *str) {
	int res = 0;
	while (is_whitespace(*str)) {
		str++;
	}
	const char *start = str;
	size_t len = 0;
	while (is_digit(*(str++))) {
		len++;
	}

	if (len > 12) {
		return -1;
	}

	while (len--) {
		int dig = *(start++) - '0';
		for (size_t i = 0; i < len; i++) {
			dig *= 10;
		}
		res += dig;
	}

	return res;
}

static int handle_special_ch(char c) {
	if (c == '\n') {
		tty_nextline();
		return 1;
	} else if (c == '\r') {
		tty_carriagereturn();
		return 1;
	}
	return -1;
}

int print(const char *restrict data, size_t length) {
	for (size_t i = 0; i < length; ) {
		size_t start = i;

		// Advance to next special character or end
		while (i < length && data[i] != '\n' && data[i] != '\r') {
			i++;
		}
		if (i > start) {
			tty_write(&data[start], i - start);
		}
		if (i < length) {
			// Must be a special character
			handle_special_ch(data[i]);
			i++;
		}
	}
	return 1;
}

int print_with_padding(const char *restrict data, size_t length, size_t padding, int left_justify) {
	if (!left_justify && length < padding) {
		for (size_t i = 0; i < padding - length; i++) {
			tty_putchar(' ');
		}
	}
	if (!print(data, length)) {
		return -1;
	}
	if (left_justify && length < padding) {
		for (size_t i = 0; i < padding - length; i++) {
			tty_putchar(' ');
		}
	}
}

/* TODO: this function is UNSAFE since it relies on \0 to determine the end of the string */
int kprintf_internal(const char *restrict format, va_list parameters) {
	int written = 0;
	int ret;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%') {
				format++;
			}
			size_t amount = 1;
			while (format[amount] && format[amount] != '%') {
				amount++;
			}
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount)) {
				return -1;
			}
			format += amount;
			written += amount;
			continue;
		}

		// At this point, format[0] == '%' AND format[1] != '%'
		// It's possible that format[1] == '\0'

		const char *format_begun_at = format++;

		int left_justify = 0;
		int zero_pad = 0;

		while (*format == '-' || *format == '0') {
			if (*format == '-') {
				left_justify = 1;
			} else if (*format == '0') {
				zero_pad = 1;
			}
			format++;
		}

		// only do zero pad if not left justify
		zero_pad = left_justify ^ zero_pad;

		// if (*format == '-') {
		// 	// left justified
		// 	format++;
		// 	left_justify = 1;
		// }

		size_t padding = atoi(format);

		// TODO: need to get the max number of characters to print

		if (*format == 'c') {
			format++;
			char c = (unsigned char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!handle_special_ch(c)) {
				tty_putchar(c);
			}
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if ((ret = print_with_padding(str, len, padding, left_justify)) < 0) {
				return ret;
			}
			written += len;
		} else if (*format == 'd') {
			format++;
			char buf[12];
			const char *str = itoa(va_arg(parameters, int), buf, 10);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if ((ret = print_with_padding(str, len, padding, left_justify)) < 0) {
				return ret;
			}
			written += len;
		} else if (*format == 'x') {
			format++;
			char buf[8];
			const char *str = itoa(va_arg(parameters, int), buf, 16);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if ((ret = print_with_padding(str, len, padding, left_justify)) < 0) {
				return ret;
			}
			written += len;
		}
		else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len)) {
				return -1;
			}
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	if (written > 0) {
		tty_move_cursor();
	}
	return written;
}

/* TODO: this function is UNSAFE since it relies on \0 to determine the end of the string */
int kprintf(const char *format, ...) {
	va_list parameters;
	va_start(parameters, format);
	int res = kprintf_internal(format, parameters);
	va_end(parameters);
	return res;
}

/* defined in kerror.h header file */
