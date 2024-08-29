#include <kernel/tty.h>
#include <kernel/kprintf.h>
// #include <kernel/arch/init.h>

void kernel_main(void) {
	tty_initialize();
	kprintf("Hello, kernel World!\n");
	// arch_init();
	/* Terminal scrolling test
	for (int i = 0; i < 32; i++)
		printf("Hello, kernel World! %d\n", i+1); */
}
