.section .data
gdtr: .short 0 /* For limit storage */
      .long 0 /* For base storage */

.section .text
/* Assembly routine to load the GDT */
.global load_gdt
.type load_gdt, @function
load_gdt:
    mov 4(%esp), %ax
    mov %ax, gdtr
    mov 8(%esp), %eax
    mov %eax, gdtr+2
    lgdt gdtr
    ret

/* Assembly routine to flush GDT table */
.global flush_gdt
.type flush_gdt, @function
flush_gdt:
    /* Reload CS register containing code selector: */
    jmp $0x08,$complete_flush

complete_flush:
    /* Reload data segment registers: */
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ret
