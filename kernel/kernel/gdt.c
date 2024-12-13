#include <kernel/gdt.h>
#include <kernel/kprintf.h>
#include <kernel/kerror.h>
#include <stdint.h>

// GDT entries
uint64_t gdt_entries[5];

uint64_t create_descriptor(uint32_t base, uint32_t limit, uint16_t flag) {
	uint64_t descriptor;

	if (limit > 0xfffff) {
		// TODO: handle error
		kerror("GDT: Cannot encode limits larger than 0xfffff");
	}

	// Create the high 32 bit segment
	descriptor  =  limit       & 0x000f0000;         // set limit bits 19:16
	descriptor |= (flag <<  8) & 0x00f0ff00;         // set type, p, dpl, s, g, d/b, l and avl fields
	descriptor |= (base >> 16) & 0x000000ff;         // set base bits 23:16
	descriptor |=  base        & 0xff000000;         // set base bits 31:24

	// Shift by 32 to allow for low part of segment
	descriptor <<= 32;

	// Create the low 32 bit segment
	descriptor |= base  << 16;                       // set base bits 15:0
	descriptor |= limit  & 0x0000ffff;               // set limit bits 15:0
	
	return descriptor;
}

void init_gdt() {
	// Null entry
	gdt_entries[0] = create_descriptor(0, 0, 0);

	// TODO: possibly create these ahead of time?
	gdt_entries[1] = create_descriptor(0, 0x000fffff, (GDT_CODE_PL0));
	gdt_entries[2] = create_descriptor(0, 0x000fffff, (GDT_DATA_PL0));
	gdt_entries[3] = create_descriptor(0, 0x000fffff, (GDT_CODE_PL3));
	gdt_entries[4] = create_descriptor(0, 0x000fffff, (GDT_DATA_PL3));
	
	// Clear interrupts
	__asm__("cli");

	// Set up the GDT table values
	const uint16_t limit = sizeof(uint64_t) * 5 - 1;
	const uint32_t base = (uint32_t)&gdt_entries;
	
	// Call assembly routine to flush the GDT
	load_gdt(limit, base);

	// Call assembly routine to flush the GDT
	flush_gdt();
	
	kprintf("GDT successfully loaded!\n");
}
