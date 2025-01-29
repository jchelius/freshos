.section .data

.section .text

/* Assembly routine to flush GDT table */
.global idt_flush
.type idt_flush, @function
.extern idt_table
idt_flush:
	lidt idt_table
	sti
	ret
