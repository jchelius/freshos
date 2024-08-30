#include <stdint.h>
#include <kernel/gdt.h>
#include <kernel/kprintf.h>

// GDT entries
uint64_t gdt_entries[5];
// gdt_entry_t gdt_entries[5];

// GDT table
gdt_table_t gdt_table;

/* extern void load_gdt(uint16_t limit, uint32_t base);
extern void flush_gdt(); */

uint64_t create_descriptor(uint32_t base, uint32_t limit, uint16_t flag)
{
    uint64_t descriptor;

    // Create the high 32 bit segment
    descriptor  =  limit       & 0x000F0000;         // set limit bits 19:16
    descriptor |= (flag <<  8) & 0x00F0FF00;         // set type, p, dpl, s, g, d/b, l and avl fields
    descriptor |= (base >> 16) & 0x000000FF;         // set base bits 23:16
    descriptor |=  base        & 0xFF000000;         // set base bits 31:24

    // Shift by 32 to allow for low part of segment
    descriptor <<= 32;

    // Create the low 32 bit segment
    descriptor |= base  << 16;                       // set base bits 15:0
    descriptor |= limit  & 0x0000FFFF;               // set limit bits 15:0
    
    return descriptor;
}

void init_gdt() {
    // Null entry
    gdt_entries[0] = create_descriptor(0, 0, 0);

    gdt_entries[1] = create_descriptor(0, 0x000fffff, (GDT_CODE_PL0));
    gdt_entries[2] = create_descriptor(0, 0x000fffff, (GDT_DATA_PL0));
    gdt_entries[3] = create_descriptor(0, 0x000fffff, (GDT_CODE_PL3));
    gdt_entries[4] = create_descriptor(0, 0x000fffff, (GDT_DATA_PL3));
    
    // Code segment 
    /* gdt_entries[1] = (gdt_entry_t) {
        .limit_low = 0xFFFF,
        .base_low = 0x0000,
        .base_middle = 0x00,
        .access = 0x9A,
        .granularity = 0xCF,
        .base_high = 0x00
    }; */

    // Data segment 
    /* gdt_entries[2] = (gdt_entry_t) {
        .limit_low = 0xFFFF,
        .base_low = 0x0000,
        .base_middle = 0x00,
        .access = 0x92,
        .granularity = 0xCF,
        .base_high = 0x00
    }; */

    // Clear interrupts
    asm("cli");

    // Set up the GDT table values
    gdt_table.limit = sizeof(uint64_t) * 5 - 1;
    gdt_table.base = (uint32_t)&gdt_entries;
    
    // Call assembly routine to flush the GDT
    load_gdt(gdt_table.limit, gdt_table.base);

    // Call assembly routine to flush the GDT
    flush_gdt();
    
    kprintf("GDT successfully loaded!\n");
    // Call assembly routine to load the GDT
    // load_gdt(&gdt_ptr);
}
