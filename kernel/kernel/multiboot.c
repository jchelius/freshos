#include <kernel/multiboot.h>
#include <kernel/memory_map.h>
#include <kernel/kstdio.h>

void multiboot_init_memory_map(struct multiboot_info *mb_info) {
	uint32_t cur = mb_info->mmap_addr;
	uint32_t end = cur + mb_info->mmap_length;

    while (cur < end) {
        struct multiboot_mmap_entry* mb_entry =
            (struct multiboot_mmap_entry*) cur;

		memory_region_type_t type;

		if (mb_entry->size < 20) {
			// TODO: panic
			kprintf("corrupted mb_entry at address=%x\n", cur);
			break;
		}

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

		if (memory_map_add_region(
			mb_entry->base_addr,
			mb_entry->length,
			type
		) < 0) {
			// TODO: panic
			kprintf("memory_map_add_region failed at base=0x%llx\n", mb_entry->base_addr);
			break;
		}

        cur += mb_entry->size + sizeof(mb_entry->size);
    }
}
