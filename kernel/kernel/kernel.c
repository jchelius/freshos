#include <kernel/tty.h>
#include <kernel/kstdio.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/multiboot.h>

// volatile int d = 0;

void kernel_main(uint32_t mb_magic, uint32_t mb_addr) {
	if (mb_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		kerror("No Multiboot Magic found.\n");
		// TODO: panic
	}
	
	tty_initialize();
	kprintf("Hello, kernel World!\n");
	kprintf("sizeof(multiboot_info)=%d\n", sizeof(struct multiboot_info));
	struct multiboot_info *mb_info = (struct multiboot_info *) mb_addr;
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
