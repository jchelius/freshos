#pragma once
#include <stdint.h>

// Refer to the Intel documentation for a description of what each one does.
#define SEG_DESCTYPE(x)   ((x) << 4)          // Descriptor type (0 for system, 1 for code/data)
#define SEG_PRES(x)       ((x) << 7)          // Present
#define SEG_SAVL(x)       ((x) << 10)         // Available for system use
#define SEG_LONG(x)       ((x) << 13)         // Long mode
#define SEG_SIZE(x)       ((x) << 14)         // Size (0 for 16-bit, 1 for 32)
#define SEG_GRAN(x)       ((x) << 15)         // Granularity (0 for 1B-4MB, 1 for 4 KB - 4 GB)
#define SEG_PRIV(x)    (((x) & 3) << 5)       // Set privilege level (0-3)

#define SEG_DATA_RD         0x00 // Read-Only
#define SEG_DATA_RDA        0x01 // Read-Only, accessed
#define SEG_DATA_RDWR       0x02 // Read/Write
#define SEG_DATA_RDWRA      0x03 // Read/Write, accessed
#define SEG_DATA_RDEXPD     0x04 // Read-Only, expand-down
#define SEG_DATA_RDEXPDA    0x05 // Read-Only, expand-down, accessed
#define SEG_DATA_RDWREXPD   0x06 // Read/Write, expand-down
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

// Define a GDT entry 
/* typedef struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed)) gdt_entry_t; */

// Define a GDT table
typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute((packed)) gdt_table_t;

void init_gdt(void);
uint64_t create_descriptor(uint32_t base, uint32_t limit, uint16_t flag);
void load_gdt(uint16_t limit, uint32_t base);
void flush_gdt();
