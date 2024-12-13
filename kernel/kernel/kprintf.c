#include <kernel/kprintf.h>

#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>

#include <kernel/tty.h>
#include <kernel/strutil.h>
#include <kernel/vga.h>
#include <kernel/kerror.h>

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

static bool handle_special_ch(char c) {
	if (c == '\n') {
		tty_nextline();
		return true;
	} else if (c == '\r') {
		tty_carriagereturn();
		return true;
	}
	return false;
}

static bool print(const char *data, size_t length) {
	for (size_t i = 0; i < length; ) {
		size_t pos = i;
		while (pos < length && data[pos] != '\n' && data[pos] != '\r') {
			pos++;
		}
		if (pos == i) {
			handle_special_ch(data[pos]);
			i++;
			continue;
		}
		tty_write(&data[i], pos - i);
		i = pos;
	}
	return true;
}

static int kprintf_internal(const char *format, va_list parameters) {
	int written = 0;

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

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
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
			if (!print(str, len)) {
				return -1;
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
			if (!print(str, len)) {
				return -1;
			}
			written += len;
		} else {
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

int kprintf(const char *format, ...) {
	va_list parameters;
	va_start(parameters, format);
	int res = kprintf_internal(format, parameters);
	va_end(parameters);
	return res;
}

/* defined in kerror.h header file */
int kerror(const char *format, ...) {
	va_list parameters;
	va_start(parameters, format);
	const uint8_t prev_color = tty_getfgcolor();
	tty_setfgcolor(VGA_COLOR_RED);
	int res = kprintf_internal(format, parameters);
	tty_setfgcolor(prev_color);
	va_end(parameters);
	return res;
}
