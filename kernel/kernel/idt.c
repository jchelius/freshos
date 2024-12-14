#include <kernel/idt.h>
#include <stdint.h>
#include <kernel/strutil.h>
#include <kernel/kprintf.h>

// Define a IDT table
struct {
	uint16_t limit;
	uint32_t base;
} __attribute((packed)) idt_table;

// IDT entries
uint64_t idt_entries[256];

extern void idt_load();

static void idt_descriptor_set(uint8_t index, uint32_t offset, uint16_t segment, uint8_t flag) {
	uint64_t *descriptor = &idt_entries[index];

	// Set the lower 16 bits of the offset
	*descriptor = offset & 0xffff;

	// Set the flag
	*descriptor |= (uint64_t)(flag) << 40;

	// Set the segment selector
	*descriptor |= (uint32_t)(segment) << 16;

	// Set the upper 16 bits of the offset
	*descriptor |= (uint64_t)(offset >> 16) << 32;
}

void idt_init() {
	idt_table.limit = sizeof(uint64_t) * 256 - 1;
	idt_table.base = (uint32_t)&idt_entries;

	memset(&idt_entries, 0, sizeof(uint64_t) * 256);

	idt_load();
	kprintf("IDT successfully loaded!\n");
}
