DEFAULT_HOST!=../default-host.sh
HOST?=DEFAULT_HOST
HOSTARCH!=../target-triplet-to-arch.sh $(HOST)

CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

OBJDIR=obj

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra -Iinclude
#CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -Iinclude
CPPFLAGS:=$(CPPFLAGS) 
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lgcc

ARCHDIR=arch/$(HOSTARCH)

#include $(ARCHDIR)/make.config

CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)

KERNEL_OBJS=\
kernel/kernel.o \
kernel/kprintf.o \
kernel/strutil/memcmp.o \
kernel/strutil/memcpy.o \
kernel/strutil/memmove.o \
kernel/strutil/memset.o \
kernel/strutil/strlen.o \
kernel/boot.o \
kernel/tty.o \
kernel/gdt.o \
kernel/gdt_asm.o

LD_FILE=kernel/linker.ld

OBJS=\
$(KERNEL_OBJS) \

LINK_LIST=\
$(LDFLAGS) \
$(addprefix $(OBJDIR)/,$(KERNEL_OBJS)) \
$(LIBS) \

.PHONY: all clean install install-headers install-kernel

all: myos.kernel

myos.kernel: $(OBJS) $(LD_FILE)
	$(CC) -T $(LD_FILE) -o $@ $(CFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot myos.kernel

%.o: %.c
	@mkdir -p $(OBJDIR)/$(@D)
	$(CC) -MD -c $< -o $(OBJDIR)/$@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

%.o: %.S
	@mkdir -p $(OBJDIR)/$(@D)
	$(CC) -MD -c $< -o $(OBJDIR)/$@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f myos.kernel
	rm -rf $(OBJDIR)

install: install-headers install-kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	# cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-kernel: myos.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp myos.kernel $(DESTDIR)$(BOOTDIR)

-include $(OBJS:.o=.d)
