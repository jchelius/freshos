#include <kernel/kstdio.h>

#include <limits.h>
#include <stdarg.h>

#include <kernel/tty.h>
#include <kernel/strutil.h>
#include <kernel/vga.h>

static enum LENGTH_MOD {
	LENGTH_MOD_HH,
	LENGTH_MOD_H,
	LENGTH_MOD_NONE,
	LENGTH_MOD_L,
	LENGTH_MOD_LL,
	LENGTH_MOD_J,
	LENGTH_MOD_Z,
	LENGTH_MOD_T,
	LENGTH_MOD_L_CAP,
};

// static char *itoa(int value, char *str, int base) {
//     char *rc;
//     char *ptr;
//     char *low;
//     // Check for supported base.
//     if (base < 2 || base > 36) {
//         *str = '\0';
//         return str;
//     }
//     rc = ptr = str;
//     // Set '-' for negative decimals.
//     if (value < 0 && base == 10) {
//         *ptr++ = '-';
//     }
//     // Remember where the numbers start.
//     low = ptr;
//     // The actual conversion.
//     do {
//         // Modulo is negative for negative value. This trick makes abs() unnecessary.
//         *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
//         value /= base;
//     } while (value);
//     // Terminating the strutil.
//     *ptr-- = '\0';
//     // Invert the numbers.
//     while (low < ptr) {
//         char tmp = *low;
//         *low++ = *ptr;
//         *ptr-- = tmp;
//     }
//     return rc;
// }

// Convert unsigned integer to string (base 2..36)
char *utoa(uintmax_t value, char *str, int base, int uppercase) {
    if (base < 2 || base > 36) {
        *str = '\0';
        return str;
    }

    char *ptr = str;
    char *low = ptr;

    // Conversion table
    const char *digits = uppercase
        ? "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        : "0123456789abcdefghijklmnopqrstuvwxyz";

    // Special case: 0
    if (value == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return str;
    }

    // Convert digits in reverse
    while (value) {
        *ptr++ = digits[value % base];
        value /= base;
    }

    *ptr-- = '\0';

    // Reverse the string
    while (low < ptr) {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }

    return str;
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
    int mul = 1;

    // Skip leading whitespace
    while (is_whitespace(*str)) {
        str++;
    }

    // Handle optional sign
    if (*str == '-') {
        mul = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }

    // Convert digits
    while (is_digit(*str)) {
        int digit = *str - '0';

        // Simple overflow check for 32-bit int
        if (res > (INT_MAX - digit) / 10) {
            return (mul == 1) ? INT_MAX : INT_MIN;
        }

        res = res * 10 + digit;
        str++;
    }

    return res * mul;
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
	return 0;
}

static int emit_string(
    const char *str,
    int min_width,
    int precision,
    int left_justify,
    int maxrem
) {
    if (!str) {
        str = "(null)";
    }

    size_t len = strlen(str);
    if (precision >= 0 && (size_t)precision < len) {
        len = precision;  // truncate to precision
    }

    if (maxrem < len || maxrem < min_width) {
        return -1; // overflow
    }

    char pad_char = ' ';

    // Right-justify padding
    if (!left_justify && (size_t)min_width > len) {
        for (size_t i = 0; i < min_width - len; i++) {
            tty_putchar(pad_char);
        }
    }

    // Print the string
    if (print(str, len) < 0) {
        return -1;
    }

    // Left-justify padding
    if (left_justify && (size_t)min_width > len) {
        for (size_t i = 0; i < min_width - len; i++) {
            tty_putchar(' ');
        }
    }

    return (len < (size_t)min_width) ? min_width : len;
}

static int emit_integer(
    uintmax_t uval,
    int is_neg,
    int base,
    int min_width,
    int precision,
    int left_justify,
    int zero_pad,
    int uppercase,
    int maxrem
) {
    char buf[32];
    size_t digit_len = 0;

    // Special case: precision=0 and value=0 -> print nothing
    if (precision == 0 && uval == 0) {
        buf[0] = '\0';
        digit_len = 0;
    } else {
        utoa(uval, buf, base, uppercase);
        digit_len = strlen(buf);
    }

    // // Apply uppercase for hex
    // if (uppercase) {
    //     for (size_t i = 0; i < digit_len; i++) {
    //         if (buf[i] >= 'a' && buf[i] <= 'f') {
    //             buf[i] -= 32;
    //         }
    //     }
    // }

    // Precision zeros
    size_t num_zeros = 0;
    if (precision > (int)digit_len) {
        num_zeros = precision - digit_len;
    }

    // Total length including sign
    size_t core_len = digit_len + num_zeros + (is_neg ? 1 : 0);

    // If precision is specified, zero_pad is ignored
    if (precision >= 0) {
        zero_pad = 0;
    }

    // Width padding
    size_t num_spaces = 0;
    if (min_width > (int)core_len) {
        num_spaces = min_width - core_len;
    }

    if (core_len + num_spaces > (size_t)maxrem) {
        return -1;
    }

    char pad_char = zero_pad ? '0' : ' ';

    // ---- Emit ----
    // 1. Leading spaces (if right-justified and zero_pad is off)
    if (!left_justify && !zero_pad) {
        for (size_t i = 0; i < num_spaces; i++) {
            tty_putchar(' ');
        }
    }

    // 2. Sign
    if (is_neg) {
        tty_putchar('-');
    }

    // 3. Zero padding from width (only if zero_pad is on)
    if (!left_justify && zero_pad) {
        for (size_t i = 0; i < num_spaces; i++) {
            tty_putchar('0');
        }
    }

    // 4. Precision zeros
    for (size_t i = 0; i < num_zeros; i++) {
        tty_putchar('0');
    }

    // 5. Digits
    if (digit_len > 0) {
        tty_write(buf, digit_len);
    }

    // 6. Trailing spaces (left-justify)
    if (left_justify) {
        for (size_t i = 0; i < num_spaces; i++) {
            tty_putchar(' ');
        }
    }

    return core_len + num_spaces;
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
			if (print(format, amount) < 0) {
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
		int precision = -1;
		enum LENGTH_MOD length_mod = LENGTH_MOD_NONE;

		while (*format == '-' || *format == '0') {
			if (*format == '-') {
				left_justify = 1;
			} else if (*format == '0') {
				zero_pad = 1;
			}
			format++;
		}

		// only do zero pad if not left justify
		// zero_pad = left_justify ^ zero_pad;

		if (precision >= 0) {
			zero_pad = 0;
		}
		if (left_justify) {
			zero_pad = 0;
		}

		char pad_char = zero_pad ? '0' : ' ';

		// if (*format == '-') {
		// 	// left justified
		// 	format++;
		// 	left_justify = 1;
		// }

		size_t min_field_width = atoi(format);

		while (is_digit(*format)) {
			format++;
		}

		if (*format == '.') {
			format++;
			precision = is_digit(*format) ? atoi(format) : 0;
			while (is_digit(*format)){
				format++;
			}
		}

		if (*format == 'h') {
			if (*(++format) == 'h') {
				length_mod = LENGTH_MOD_HH;
				format++;
			} else {
				length_mod = LENGTH_MOD_H;
			}
		} else if (*format == 'l') {
			if (*(++format) == 'l') {
				length_mod = LENGTH_MOD_LL;
				format++;
			} else {
				length_mod = LENGTH_MOD_L;
			}
		} else if (*format == 'j') {
			length_mod = LENGTH_MOD_J;
			format++;
		} else if (*format == 'z') {
			length_mod = LENGTH_MOD_Z;
			format++;
		} else if (*format == 't') {
			length_mod = LENGTH_MOD_T;
			format++;
		} else if (*format == 'L') {
			length_mod = LENGTH_MOD_L_CAP;
			format++;
		}

		// TODO: need to get the max number of characters to print

		switch (*format) {
			case 'c': {
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
				break;
			}
			case 'd' : {
				format++;
				if (precision < 0) {
					precision = 1;
				}
				char buf[21];
				// char buf[12];
				intmax_t val;
				uintmax_t uval;
				int is_neg = 0;
				switch (length_mod) {
					case LENGTH_MOD_HH:
						val = (char) va_arg(parameters, int);
						break;
					case LENGTH_MOD_H:
						val = (short) va_arg(parameters, int);
						break;
					case LENGTH_MOD_L:
						val = va_arg(parameters, long);
						break;
					case LENGTH_MOD_LL:
						val = va_arg(parameters, long long);
						break;
					case LENGTH_MOD_J:
						val = va_arg(parameters, intmax_t);
						break;
					case LENGTH_MOD_Z:
						val = va_arg(parameters, ssize_t);
						break;
					case LENGTH_MOD_T:
						val = va_arg(parameters, ptrdiff_t);
						break;
					default:
						val = va_arg(parameters, int);
				}
				
				if (val < 0) {
					is_neg = 1;
					uval = (uintmax_t)(-(val + 1)) + 1;  // INT_MIN safe
				} else {
					uval = (uintmax_t)val;
				}

				if ((ret = emit_integer(
					uval,
					is_neg,
					10,
					min_field_width,
					precision,
					left_justify,
					zero_pad,
					0,
					maxrem
				)) < 0) {
					// TODO: Set errno to EOVERFLOW.
					return -1;
				}
				written += ret;
				break;
			}
			case 'x':
			case 'X':
			case 'u': {
				char conv = *format++;
				int uppercase = (conv == 'X');
				int base = (conv == 'u') ? 10 : 16;
				char buf[21];
				// char buf[8];
				uintmax_t uval;
				switch (length_mod) {
					case LENGTH_MOD_HH:
						uval = (unsigned char) va_arg(parameters, int);
						break;
					case LENGTH_MOD_H:
						uval = (unsigned short) va_arg(parameters, int);
						break;
					case LENGTH_MOD_L:
						uval = va_arg(parameters, unsigned long);
						break;
					case LENGTH_MOD_LL:
						uval = va_arg(parameters, unsigned long long);
						break;
					case LENGTH_MOD_J:
						uval = va_arg(parameters, uintmax_t);
						break;
					case LENGTH_MOD_Z:
						uval = va_arg(parameters, size_t);
						break;
					case LENGTH_MOD_T:
						uval = va_arg(parameters, uintptr_t);
						break;
					default:
						uval = (unsigned int) va_arg(parameters, int);
				}
				
				if ((ret = emit_integer(
					uval,
					0,
					base,
					min_field_width,
					precision,
					left_justify,
					zero_pad,
					uppercase,
					maxrem
				)) < 0) {
					// TODO: Set errno to EOVERFLOW.
					return -1;
				}
				written += ret;
				break;
			}
			case 's':
			default: {
				format++;
				const char* str = va_arg(parameters, const char*);
				if ((ret = emit_string(
					str,
					min_field_width,
					precision,
					left_justify,
					zero_pad,
					maxrem
				)) < 0) {
					// TODO: Set errno to EOVERFLOW.
					return -1;
				}
				written += ret;
			}

		}
	}

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
