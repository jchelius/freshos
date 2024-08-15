#include <stdarg.h>
#include <stdint.h>

#include "terminal.h"

static void print_char(char ch){
	if (ch == '\n'){
		terminal_nextline();
		return;
	}
	terminal_putchar(ch);
}

static void print_string(const char *str) {
    while (*str) print_char(*str++);
}

static void print_int(int val) {
    char buf[10];
    char *p = buf + sizeof(buf) - 1;
    int negative = 0;

    if (val < 0) {
        negative = 1;
        val = -val;
    }

    *p = '\0';
    do {
        *(--p) = '0' + (val % 10);
        val /= 10;
    } while (val);

    if (negative) {
        *(--p) = '-';
    }

    print_string(p);
}

int kprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 'd':
                    print_int(va_arg(args, int));
                    break;
                case 's':
                    print_string(va_arg(args, char *));
                    break;
                default:
                    terminal_putchar('%');
                    print_char(*fmt);
                    break;
            }
        } else {
            print_char(*fmt);
        }
        fmt++;
    }

    va_end(args);
    return 0;
}
