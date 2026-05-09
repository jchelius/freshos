#pragma once
#include <stdint.h>
#include <stddef.h>
#include <kernel/multiboot.h>

#define MAX_MEMORY_REGIONS 128

typedef enum {
    MEMORY_USABLE,
    MEMORY_RESERVED,
    MEMORY_ACPI_RECLAIMABLE,
    MEMORY_ACPI_NVS,
    MEMORY_BADRAM,
    MEMORY_KERNEL,
    MEMORY_BOOTLOADER,
    MEMORY_FRAMEBUFFER
} memory_region_type_t;

typedef struct {
	uint64_t base;
	uint64_t length;
	memory_region_type_t type;
} memory_region_t;

typedef struct {
	memory_region_t regions[MAX_MEMORY_REGIONS];
	size_t region_count;

	uint64_t total_memory;
	uint64_t usable_memory;
} memory_map_t;

static memory_map_t memory_map;

int memory_map_add_region(
	uint64_t base,
	uint64_t length,
	memory_region_type_t type
);

const memory_map_t *memory_map_get(void) {
	return &memory_map;
}

int memory_map_init_from_multiboot(struct multiboot_info *mb_info);
