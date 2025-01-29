#include <kernel/kstdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <kernel/tty.h>
#include <kernel/vga.h>

extern int kprintf_internal(const char *restrict format, va_list parameters);

int kerror(const char *restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);
	const uint8_t prev_color = tty_getfgcolor();
	tty_setfgcolor(VGA_COLOR_RED);
	int res = kprintf_internal(format, parameters);
	tty_setfgcolor(prev_color);
	va_end(parameters);
	return res;
}
