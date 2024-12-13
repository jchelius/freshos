#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

void tty_initialize();

void tty_move_cursor();
void tty_clear();

void tty_scroll(uint8_t num_lines);
void tty_nextline();
void tty_carriagereturn();

void tty_putchar(char c);
void tty_write(const char *data, size_t size);
void tty_writestring(const char *data);

uint8_t tty_getfgcolor();
uint8_t tty_getbgcolor();
void tty_setfgcolor(uint8_t fg);
