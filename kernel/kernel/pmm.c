#include <kernel/pmm.h>
#include <kernel/multiboot.h>

static void sort_regions() {
	for (size_t i = 1; i < memory_map.region_count; ++i) {
		for (size_t j = 0; j < i; ++j) {
			if (regions[i - (j + 1)] < regions[i]) {
				break;
			}
		}
		memory_region_t tmp = memory_map.regions[i];
		memory_map.regions[i] = memory_map.regions[j];
		memory_map.regions[j] = tmp;
	}
}

static void merge_regions() {
}

static void memory_map_init_from_multiboot(uint32_t mb_addr) {
	kprintf("sizeof(multiboot_info)=%d\n", sizeof(struct multiboot_info));
	kprintf("sizeof(multiboot_mmap_entry)=%d\n", sizeof(struct multiboot_mmap_entry));
	struct multiboot_info *mb_info = (struct multiboot_info *) mb_addr;

	if (mb_info->flags & (1 << 0)) {
		kprintf("mem_lower = %dKB, mem_upper = %dKB\n",
			mb_info->mem_lower, mb_info->mem_upper);
	}

	if (mb_info->flags & (1 << 6)) {
		uint32_t mmap_addr_cur = mb_info->mmap_addr;
		uint32_t mmap_addr_end = mb_info->mmap_addr + mb_info->mmap_length;
		struct multiboot_mmap_entry *mmap_entry_cur;
		// kprintf("mmap_addr = 0x%x, mmap_length = 0x%x\n",
		//       mb_info->mmap_addr, mb_info->mmap_length);
		kprintf("mmap_addr = %d, mmap_length = %d\n",
		      mb_info->mmap_addr, mb_info->mmap_length);
		while (mmap_addr_cur < mmap_addr_end) {
			kprintf(" mmap_addr_cur=%x\n", mmap_addr_cur);
			mmap_entry_cur = (struct multiboot_mmap_entry *) mmap_addr_cur;
			kprintf(" mmap_addr_cur->size + sizeof(mmap_addr_cur->size)=%d\n", mmap_entry_cur->size + sizeof(mmap_entry_cur->size));
			kprintf(" size %u, base_addr = %llx,"
				" length = %llu, type = %u\n",
				mmap_entry_cur->size,
				mmap_entry_cur->base_addr,
				mmap_entry_cur->length,
				mmap_entry_cur->type);

			// add region

			if (memory_map.region_count >= MAX_MEMORY_REGIONS) {
				break;
			}
			memory_region_t *region = &memory_map.regions[memory_map.region_count++];
			region->start = mmap_entry_cur->base_addr;
			region->end = region->start + mmap_entry_cur->length;
			regon->type = (memory_region_type_t) type;

			mmap_addr_cur += (mmap_entry_cur->size + sizeof(mmap_entry_cur->size));
		}
	}
}

void pmm_init(uint32_t mb_addr) {
	memory_map_init_from_multiboot(mb_addr);
	// sort
	sort_regions();
	// merge
	merge_regions();	
	
}
