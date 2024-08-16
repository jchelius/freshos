#include <stdio.h>

#if defined(__is_libk)
#include <kernel/tty.h>
#endif

int putchar(int ic) {
#if defined(__is_libk)
	char c = (char) ic;
	if (ic == '\n') {
		tty_nextline();
		goto ret;
	}
	tty_write(&c, sizeof(c));
#else
	// TODO: Implement stdio and the write system call.
#endif
ret:
	return ic;
}
