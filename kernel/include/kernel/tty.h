#pragma once

#include <stddef.h>
#include <stdint.h>

void tty_initialize(void);
void tty_putchar_color(char c, uint8_t color);
void tty_putchar(char c);
void tty_write_color(const char *data, size_t size, uint8_t color);
void tty_write(const char *data, size_t size);
void tty_writestring_color(const char *data, uint8_t color);
void tty_writestring(const char *data);
uint8_t tty_getcolor();
void tty_scroll();
void tty_nextline();
