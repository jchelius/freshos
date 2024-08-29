; Assembly routine to load the GDT
global load_gdt
load_gdt:
	lgdt[eax] ; Load GDT pointer from address in EAX
	ret

; Assembly routine to load and flush flush GDT table
global 
flush_gdt:
	lgdt[eax] ; Load GDT pointer from address in EAX
	jmp 0x08:complete_flush

complete_flush:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	ret
