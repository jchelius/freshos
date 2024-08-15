#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Hardware text mode color constants. */
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

/* increment current terminal line */
void terminal_nextline();

/* initialize terminal */
void terminal_initialize (void);

/*set current color (foreground and background ) of terminal */
void terminal_setcolor (uint8_t color);

/* put char at position with given color */
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);

/* put char at current position with current foreground color */
void terminal_putchar (char c);

/* write buffer to terminal at current position with current foreground color */
void terminal_write (const char *data, size_t size);

/* write string to terminal at current position with current foreground color */
void terminal_writestring (const char *data);
