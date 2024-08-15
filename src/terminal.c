#include "terminal.h"
#include "strutil.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t *terminal_buffer;

/* get vga color */
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

/* get vga entry */
static inline uint16_t vga_entry (unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

/* simple function to get 2d index into 1d index */
static inline size_t get_index (size_t x, size_t y) {
    return y * VGA_WIDTH + x;
}

void terminal_nextline(){
   terminal_column = 0;
   if (++terminal_row == VGA_HEIGHT)
       terminal_row = 0;
}

void terminal_initialize (void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*) 0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++){
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = get_index(x, y);
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor (uint8_t color) {
    terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = get_index(x, y);
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar (char c) {
    if (c == '\n'){
        terminal_nextline();
        return;
    }
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
        terminal_nextline();
    }
}

void terminal_write (const char *data, size_t size) {
    for (size_t i = 0; i < size; i++)
        terminal_putchar(data[i]);
}

void terminal_writestring (const char *data) {
    terminal_write(data, strlen(data));
}
