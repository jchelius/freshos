#pragma once
#include <stddef.h>

#define MAX_MEMORY_REGIONS 128

typedef enum {
	MEMORY_USABLE = 1,
	MEMORY_RESERVED = 2,
	MEMORY_ACPI_RECLAIMABLE = 3,
	MEMORY_ACPI_NVS = 4,
	MEMORY_BADRAM = 5,
	MEMORY_KERNEL = 6,
	MEMORY_BOOTLOADER = 7,
	MEMORY_FRAMEBUFFER = 8
} memory_region_type_t;

typedef struct {
	uint64_t start;
	uint64_t end;
	memory_region_type_t type;
} memory_region_t;

typedef struct {
	memory_region_t regions[MAX_MEMORY_REGIONS];
	size_t region_count;
	
	uint64_t total_memory;
	uint64_t usable_memory;
} memory_map_t;

memory_map_t memory_map;

void pmm_init(uint32_t mb_addr);
