#include <kernel/tty.h>
#include <kernel/kprintf.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>

void kernel_main(void) {
	tty_initialize();
	kprintf("Hello, kernel World!\n");
	gdt_init();
	idt_init();
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
