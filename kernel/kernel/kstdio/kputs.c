#include <kernel/kstdio.h>
#include <kernel/strutil.h>
#include <stddef.h>
#include <kernel/tty.h>

extern int print(const char *restrict str, size_t length);

int kputs(const char *restrict str) {
	size_t len = strlen(str);
	int ret = print(str, len);
	tty_nextline();
	return ret;
}
