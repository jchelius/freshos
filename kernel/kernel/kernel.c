#include <kernel/tty.h>
#include <kernel/kstdio.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/multiboot.h>

// volatile int d = 0;

extern void kprintf_test_all(void);

void kernel_main(uint32_t mb_magic, uint32_t mb_addr) {
	if (mb_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		kerror("No Multiboot Magic found.\n");
		// TODO: panic
	}
	
	tty_initialize();

	kprintf_test_all();

	while (1) {
		__asm__ volatile("hlt");
	}

	kprintf("Hello, kernel World!\n");

	kprintf("sizeof(multiboot_info)=%d\n", sizeof(struct multiboot_info));
	kprintf("sizeof(multiboot_mmap_entry)=%d\n", sizeof(struct multiboot_mmap_entry));
	struct multiboot_info *mb_info = (struct multiboot_info *) mb_addr;

	if (mb_info->flags & (1 << 0)) {
		kprintf("mem_lower = %dKB, mem_upper = %dKB\n",
			mb_info->mem_lower, mb_info->mem_upper);
	}

	if (mb_info->flags & (1 << 6)) {
		uint32_t mmap_len_tot = mb_info->mmap_length;
		uint32_t mmap_addr_cur = mb_info->mmap_addr;
		struct multiboot_mmap_entry *mmap_entry_cur;
		// kprintf("mmap_addr = 0x%x, mmap_length = 0x%x\n",
		//       mb_info->mmap_addr, mb_info->mmap_length);
		kprintf("mmap_addr = %d, mmap_length = %d\n",
		      mb_info->mmap_addr, mb_info->mmap_length);
		do {
			kprintf(" mmap_addr_cur=%d\n", mmap_addr_cur);
			mmap_entry_cur = (struct multiboot_mmap_entry *) mmap_addr_cur;
			kprintf(" mmap_addr_cur->size + sizeof(mmap_addr_cur->size)=%d\n", mmap_entry_cur->size + sizeof(mmap_entry_cur->size));
			kprintf(" size %d, base_addr = %d %d,"
				" length = %d %d, type = %d\n",
				mmap_entry_cur->size,
				(uint32_t) (mmap_entry_cur->base_addr >> 32),
				(uint32_t) (mmap_entry_cur->base_addr & 0xffffffff),
				(uint32_t) (mmap_entry_cur->length >> 32),
				(uint32_t) (mmap_entry_cur->length & 0xffffffff),
				(uint32_t) mmap_entry_cur->type);
		} while ((mmap_addr_cur = mmap_addr_cur + mmap_entry_cur->size + sizeof(mmap_entry_cur->size)) < mb_info->mmap_addr + mmap_len_tot);
	}

	gdt_init();
	idt_init();

	while (1) {
		__asm__ volatile("hlt");
	}

	/* Interrupt test: Divide by zero
	 * this should cause an exception */
	// int x = 10 / d;
	// kprintf("the result is %d\n", x);

	/* Terminal scrolling test */
	// char test_buf[26] = "Hello, kernel World! %d \n";
	// for (int i = 0; i < 32; i++) {
	// 	kprintf(test_buf, i+1);
	// 	for (size_t j = 0; j < 19; j++) {
	// 		test_buf[j] = ((test_buf[j] + 1) - 'A') % ('z' + 1 - 'A') + 'A';
	// 		if (test_buf[j] == '%') {
	// 			test_buf[j] = ((test_buf[j] + 1) - 'A') % ('z' + 1 - 'A') + 'A';
	// 		}
	// 	}
	// }
}
