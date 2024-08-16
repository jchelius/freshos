#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void) {
	tty_initialize();
	printf("Hello, kernel World!\n");
	/* Terminal scrolling test
	for (int i = 0; i < 32; i++)
		printf("Hello, kernel World! %d\n", i+1); */
}
