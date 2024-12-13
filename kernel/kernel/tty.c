#include <kernel/tty.h>
#include <kernel/strutil.h>

#include <kernel/vga.h>
#include <system.h>

static const uint8_t VGA_WIDTH = 80;
static const uint8_t VGA_HEIGHT = 25;
static const uint16_t VGA_MAX_INDEX = VGA_HEIGHT * VGA_WIDTH - 1;
static const uint16_t VGA_LAST_LINE_INDEX = (VGA_HEIGHT - 1) * VGA_WIDTH;
static uint16_t *const VGA_MEMORY = (uint16_t *) 0xB8000;
static const uint8_t BG_COLOR = VGA_COLOR_BLACK;

static uint16_t tty_pos;
static uint8_t tty_color;
static uint16_t *tty_buffer;

void tty_initialize() {
	tty_setfgcolor(VGA_COLOR_LIGHT_GREY);
	tty_buffer = VGA_MEMORY;
	tty_clear();
}

void tty_move_cursor() {
	outportb(0x3d4, 14);
	outportb(0x3d5, tty_pos >> 8);
	outportb(0x3d4, 15);
	outportb(0x3d5, (uint8_t)tty_pos);
}

void tty_clear() {
	for (uint16_t i = 0; i <= VGA_MAX_INDEX; i++) {
		tty_buffer[i] = vga_entry(' ', tty_color);
	}
	tty_pos = 0;
	tty_move_cursor();
}

void tty_putentryat(uint8_t ch, uint8_t col, uint8_t row) {
	if (col >= VGA_WIDTH || row >= VGA_HEIGHT) {
		return;
	}
	const size_t index = row * VGA_WIDTH + col;
	tty_buffer[index] = vga_entry(ch, tty_color);
}

void tty_scroll(uint8_t num_lines) {
	/* Copy each line, except the first, to the address in which
	 * the previous line was stored */

	const uint16_t stride = VGA_WIDTH * num_lines;
	// TODO: better cache locality?
	for (uint16_t i = 0; i < (VGA_HEIGHT - num_lines) * VGA_WIDTH; i++) {
		tty_buffer[i] = tty_buffer[i + stride];
	}

	/* Clear the last lines */
	for (uint16_t i = VGA_WIDTH * (VGA_HEIGHT - num_lines); i <= VGA_MAX_INDEX; i++) {
		tty_buffer[i] = vga_entry(' ', tty_color);
	}
}

void tty_nextline() {
	if (tty_pos >= VGA_LAST_LINE_INDEX) {
		tty_scroll(1);
		tty_pos = VGA_LAST_LINE_INDEX;
	} else {
		tty_pos = (tty_pos / VGA_WIDTH + 1) * VGA_WIDTH;
	}
}

void tty_carriagereturn() {
	tty_pos = tty_pos / VGA_WIDTH;
}

void tty_putchar(char c) {
	if (tty_pos > VGA_MAX_INDEX) {
		tty_scroll(1);
		tty_pos = VGA_LAST_LINE_INDEX;
	}
	tty_buffer[tty_pos++] = vga_entry((uint8_t)c, tty_color);
}

void tty_write(const char *data, size_t size) {
	if (size > VGA_MAX_INDEX + 1) {
		// shift the data to fit in the buffer
		data += (size - VGA_MAX_INDEX);
		tty_pos = 0;
	} else if (tty_pos + size > VGA_MAX_INDEX + 1) {
		// scroll to fit
		uint8_t bytes_left = (uint8_t)(size - (VGA_MAX_INDEX + 1 - tty_pos));
		uint8_t num_scrolls = bytes_left / VGA_WIDTH;
		tty_scroll(num_scrolls);
		tty_pos = VGA_MAX_INDEX + 1 - VGA_WIDTH * num_scrolls;
	}
	for (size_t i = 0; i < size; i++) {
		tty_buffer[tty_pos++] = vga_entry((uint8_t)data[i], tty_color);
	}
}

void tty_writestring(const char* data) {
	tty_write(data, strlen(data));
}

// TODO: see about making the following three functions inline
uint8_t tty_getcolor() {
	return tty_color;
}

uint8_t tty_getbgcolor() {
	return tty_color >> 4;
}

uint8_t tty_getfgcolor() {
	return tty_color & 0x0f;
}

void tty_setfgcolor(uint8_t fg) {
	tty_color = vga_entry_color(fg, BG_COLOR);
}
