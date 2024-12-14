.section .data

.section .text

/* Assembly routine to flush GDT table */
.global idt_load
.type idt_load, @function
.extern idt_table
idt_load:
	lidt idt_table
	ret
