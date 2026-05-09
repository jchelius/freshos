#include <kernel/memory_map.h>
#include <kernel/kstdio.h>

int memory_map_add_region(
	uint64_t base,
	uint64_t length,
	memory_region_type_t type
) {
	uint64_t end = base + length;

	uint64_t new_base = base;
	uint64_t new_end = end;

	for (size_t i = 0; i < memory_map.region_count; ++i) {
		memory_region_t *region = &memory_map.regions[i];

		if (region->type != type) {
			continue;
		}

		uint64_t region_end = region->base + region->length;

		// check for overlap
		if (region_end >= base && region->base <= end) {
			if (region->base < new_base) {
				// extend new_base
				new_base = region->base;
			}
			if (region_end > new_end) {
				new_end = region_end;
			}

			if (region->type == MEMORY_USABLE) {
				memory_map.usable_memory -= region->length;
			}
			memory_map.total_memory -= region->length;

			// O(1) deletion: swap end with region
			*region = memory_map.regions[--memory_map.region_count];
			--i;
		}
	}

	if (memory_map.region_count >= MAX_MEMORY_REGIONS) {
		return -1;
	}

	memory_region_t *region = &memory_map.regions[memory_map.region_count++];
	region->base = new_base;
	region->length = new_end - new_base;
	region->type = type;

	if (type == MEMORY_USABLE) {
		memory_map.usable_memory += (new_end - new_base); 
	}
	memory_map.total_memory += (new_end - new_base);

	return 0;
}

static const char* memory_type_str(memory_region_type_t type) {
    switch (type) {
        case MEMORY_USABLE: return "USABLE";
        case MEMORY_RESERVED: return "RESERVED";
        case MEMORY_ACPI_RECLAIMABLE: return "ACPI_RECL";
        case MEMORY_ACPI_NVS: return "ACPI_NVS";
        case MEMORY_BADRAM: return "BADRAM";
        default: return "UNKNOWN";
    }
}

static void memory_map_init_common() {
	memory_map.region_count = 0;
	memory_map.total_memory = 0;
	memory_map.usable_memory = 0;
}

static void memory_map_print(void) {
    kprintf("\n==== MEMORY MAP ====\n");
    kprintf(" IDX  TYPE        START              END                SIZE\n");
    kprintf("---------------------------------------------------------------\n");

    for (size_t i = 0; i < memory_map.region_count; i++) {
        memory_region_t *r = &memory_map.regions[i];

        uint64_t end = r->base + r->length;

        kprintf(" %3d  %-10s  0x%016llx  0x%016llx  %6lld KB\n",
            (int)i,
            memory_type_str(r->type),
            r->base,
            end,
            r->length / 1024
		);
    }

    kprintf("---------------------------------------------------------------\n");
    kprintf("TOTAL: %lld KB | USABLE: %lld KB\n",
        memory_map.total_memory / 1024,
        memory_map.usable_memory / 1024
	);

    kprintf("====================\n\n");
}

int memory_map_init_from_multiboot(struct multiboot_info *mb_info) {
	if (!(mb_info->flags & (1 << MULTIBOOT_MMAP_DEFINED_BIT))) {
		return -1;
	}

    kprintf("==== MULTIBOOT MEMORY MAP PARSE ====\n");
    kprintf("mmap_addr=0x%x mmap_length=%d\n",
        mb_info->mmap_addr,
        mb_info->mmap_length);

	memory_map_init_common();

	uintptr_t cur = (uintptr_t) mb_info->mmap_addr;
	uintptr_t end = cur + mb_info->mmap_length;

	int idx = 0;

    while (cur < end) {
        struct multiboot_mmap_entry* mb_entry =
            (struct multiboot_mmap_entry*) cur;

		if (mb_entry->size < 20) {
			// TODO: panic
			kprintf("corrupted mb_entry at address=%x\n", cur);
			break;
		}

		memory_region_type_t type;

		switch (mb_entry->type) {
			case 1:
				type = MEMORY_USABLE;
				break;
			case 3: 
				type = MEMORY_ACPI_RECLAIMABLE;
				break;
			case 4: 
				type = MEMORY_ACPI_NVS;
				break;
			case 5: 
				type = MEMORY_BADRAM;
				break;
			default: 
				type = MEMORY_RESERVED;
				break;
		}

		memory_map_add_region(
			mb_entry->base_addr,
			mb_entry->length,
			type
		);

        // Clean debug print (no duplication of raw struct dump)
        kprintf("[%2d] %-10s 0x%016llx - 0x%016llx (%lld KB)\n",
            idx++,
            memory_type_str(type),
            mb_entry->base_addr,
            mb_entry->base_addr + mb_entry->length,
            mb_entry->length / 1024
		);

        cur += mb_entry->size + sizeof(mb_entry->size);
	}

    kprintf("==== END MULTIBOOT MEMORY MAP PARSE ====\n");

	memory_map_print();

	return 0;
}

