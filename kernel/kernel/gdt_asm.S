.section .text

/* Assembly routine to flush GDT table */
.global gdt_flush
.type gdt_flush, @function
.extern gdt_table
gdt_flush:
	lgdt gdt_table
	/* Reload CS register containing code selector: */
	jmp $0x08, $complete_flush
complete_flush:
	/* Reload data segment registers: */
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	ret
