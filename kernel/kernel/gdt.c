#include <kernel/gdt.h>
#include <kernel/kstdio.h>
#include <stdint.h>

// Refer to the Intel documentation for a description of what each one does.
#define SEG_DESCTYPE(x)   ((x) << 4)          // Descriptor type (0 for system, 1 for code/data)
#define SEG_PRES(x)       ((x) << 7)          // Present
#define SEG_SAVL(x)       ((x) << 10)         // Available for system use
#define SEG_LONG(x)       ((x) << 13)         // Long mode
#define SEG_SIZE(x)       ((x) << 14)         // Size (0 for 16-bit, 1 for 32)
#define SEG_GRAN(x)       ((x) << 15)         // Granularity (0 for 1B-4MB, 1 for 4 KB - 4 GB)
#define SEG_PRIV(x)    (((x) & 3) << 5)       // Set privilege level (0-3)

#define SEG_DATA_RD			0x00 // Read-Only
#define SEG_DATA_RDA		0x01 // Read-Only, accessed
#define SEG_DATA_RDWR		0x02 // Read/Write
#define SEG_DATA_RDWRA		0x03 // Read/Write, accessed
#define SEG_DATA_RDEXPD		0x04 // Read-Only, expand-down
#define SEG_DATA_RDEXPDA	0x05 // Read-Only, expand-down, accessed
#define SEG_DATA_RDWREXPD	0x06 // Read/Write, expand-down
#define SEG_DATA_RDWREXPDA  0x07 // Read/Write, expand-down, accessed
#define SEG_CODE_EX         0x08 // Execute-only
#define SEG_CODE_EXA        0x09 // Execute-only, accessed
#define SEG_CODE_EXRD       0x0a // Execute/Read
#define SEG_CODE_EXRDA      0x0b // Execute/Read, accessed
#define SEG_CODE_EXC        0x0c // Execute-Only, conforming
#define SEG_CODE_EXCA       0x0d // Execute-Only, conforming, accessed
#define SEG_CODE_EXRDC      0x0e // Execute/Read, conforming
#define SEG_CODE_EXRDCA     0x0f // Execute/Read, conforming, accessed

#define GDT_CODE_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
						SEG_LONG(0)  | SEG_SIZE(1) | SEG_GRAN(1) | \
						SEG_PRIV(0)  | SEG_CODE_EXRD

#define GDT_DATA_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
						SEG_LONG(0)  | SEG_SIZE(1) | SEG_GRAN(1) | \
						SEG_PRIV(0)  | SEG_DATA_RDWR

#define GDT_CODE_PL3 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
						SEG_LONG(0)  | SEG_SIZE(1) | SEG_GRAN(1) | \
						SEG_PRIV(3)  | SEG_CODE_EXRD

#define GDT_DATA_PL3 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
						SEG_LONG(0)  | SEG_SIZE(1) | SEG_GRAN(1) | \
						SEG_PRIV(3)  | SEG_DATA_RDWR

#define SEG_KERNEL_CODE 0x08
#define SEG_KERNEL_DATA 0x16
#define SEG_USER_CODE 0x24
#define SEG_USER_DATA 0x32

// Define a GDT table
struct {
	uint16_t limit;
	uint32_t base;
} __attribute((packed)) gdt_table;

// GDT entries
uint64_t gdt_entries[5];

extern void gdt_flush();

static void gdt_descriptor_set(uint8_t index, uint32_t base, uint32_t limit, uint16_t flag) {
	uint64_t *descriptor = &gdt_entries[index];

	if (limit > 0xfffff) {
		// TODO: handle error
		kerror("GDT: Cannot encode limits larger than 0xfffff");
	}

	// Create the high 32 bit segment
	*descriptor  =  limit       & 0x000f0000;         // set limit bits 19:16
	*descriptor |= (flag <<  8) & 0x00f0ff00;         // set type, p, dpl, s, g, d/b, l and avl fields
	*descriptor |= (base >> 16) & 0x000000ff;         // set base bits 23:16
	*descriptor |=  base        & 0xff000000;         // set base bits 31:24

	// Shift by 32 to allow for low part of segment
	*descriptor <<= 32;

	// Create the low 32 bit segment
	*descriptor |= base  << 16;                       // set base bits 15:0
	*descriptor |= limit  & 0x0000ffff;               // set limit bits 15:0
}

void gdt_init() {
	// Null entry
	gdt_descriptor_set(0, 0, 0, 0);

	// TODO: possibly create these ahead of time?
	gdt_descriptor_set(1, 0, 0x000fffff, (GDT_CODE_PL0));
	gdt_descriptor_set(2, 0, 0x000fffff, (GDT_DATA_PL0));
	gdt_descriptor_set(3, 0, 0x000fffff, (GDT_CODE_PL3));
	gdt_descriptor_set(4, 0, 0x000fffff, (GDT_DATA_PL3));
	
	// Clear interrupts
	__asm__("cli");

	// Set up the GDT table values
	gdt_table.limit = sizeof(uint64_t) * 5 - 1;
	gdt_table.base = (uint32_t)&gdt_entries;
	
	// Call assembly routine to flush the GDT
	gdt_flush();
	
	kprintf("GDT successfully loaded!\n");
}
