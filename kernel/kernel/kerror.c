#include <kernel/kerror.h>
#include <kernel/kprintf.h>
#include <kernel/vga.h>

int kerror(const char *restrict format, ...) {
    return kprintf_color(VGA_COLOR_RED, format);
}
