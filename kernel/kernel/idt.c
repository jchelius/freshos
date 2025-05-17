#include <kernel/idt.h>
#include <stdint.h>
#include <kernel/strutil.h>
#include <kernel/kstdio.h>
#include <kernel/gdt.h>
#include <system.h>

#define NUM_ISRS 256

#define IDT_GATE_TYPE_INTERRUPT_32 0xe
#define IDT_GATE_TYPE_TRAP_32 0xf
#define IDT_GATE_DPL(x)   ((x) << 5) // CPU Privilege level allowed to call interrupt using INT 
#define IDT_GATE_PRESENT 0x8 << 4

#define IDT_INTERRUPT_KERNEL_32 IDT_GATE_PRESENT | IDT_GATE_DPL(0) | IDT_GATE_TYPE_INTERRUPT_32
#define IDT_TRAP_KERNEL_32 IDT_GATE_PRESENT | IDT_GATE_DPL(0) | IDT_GATE_TYPE_TRAP_32

// Define a IDT table
struct {
	uint16_t limit;
	uint32_t base;
} __attribute((packed)) idt_table;

// IDT entries
uint64_t idt_entries[NUM_ISRS];
extern uint32_t isrs[NUM_ISRS];

extern void idt_flush();

char *exception_msgs[] = {
	"Division Error",
	"Debug",
	"Non-maskable Interrupt",
	"Breakpoint",
	"Overflow",
	"Bound Range Exceeded",
	"Invalid Opcode",
	"Device Not Available",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Invalid TSS",
	"Segment Not Present",
	"Stack-Segment Fault",
	"General Protection Fault",
	"Page Fault",
	"Reserved",
	"x87 Floating-point Exception",
	"Alignment Check",
	"Machine Check",
	"SIMD Floating-point Exception",
	"Virtualization Exception",
	"Control Protection Exception",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Hypervisor Injection Exception",
	"VMM Communication Exception",
	"Security Exception",
	"Reserved",
};

static void idt_descriptor_set(uint8_t index, uint32_t offset, uint16_t segment, uint8_t flag) {
	uint64_t *descriptor = &idt_entries[index];

	// Set the lower 16 bits of the offset
	*descriptor = offset & 0xffff;

	// Set the flag
	*descriptor |= (uint64_t)(flag) << 40;

	// Set the segment selector
	*descriptor |= (uint32_t)(segment) << 16;

	// Set the upper 16 bits of the offset
	*descriptor |= (uint64_t)(offset >> 16) << 48;
}

void idt_init() {
	idt_table.limit = sizeof(uint64_t) * NUM_ISRS - 1;
	idt_table.base = (uint32_t)&idt_entries;

	// clear IDT
	memset(&idt_entries, 0, sizeof(uint64_t) * 256);

	/* 0: Divide by zero: interrupt */
	idt_descriptor_set(0, isrs[0], SEG_KERNEL_CODE, IDT_INTERRUPT_KERNEL_32);
	/* 1: Debug Exception: trap */
	idt_descriptor_set(1, isrs[1], SEG_KERNEL_CODE, IDT_TRAP_KERNEL_32);
	/* 2: NMI: interrupt */
	idt_descriptor_set(2, isrs[2], SEG_KERNEL_CODE, IDT_INTERRUPT_KERNEL_32);
	/* 3: Breakpoint Exception (BP): trap */
	idt_descriptor_set(3, isrs[3], SEG_KERNEL_CODE, IDT_TRAP_KERNEL_32);
	/* 4: Overflow Exception (OF): trap */
	idt_descriptor_set(4, isrs[4], SEG_KERNEL_CODE, IDT_TRAP_KERNEL_32);
	/* 5: BOUND Range Exceeded (BR): trap */
	idt_descriptor_set(5, isrs[5], SEG_KERNEL_CODE, IDT_TRAP_KERNEL_32);
	/* 6-48: interrupt */
	for (uint8_t i = 6; i < 48; i++) {
		idt_descriptor_set(i, isrs[i], SEG_KERNEL_CODE, IDT_INTERRUPT_KERNEL_32);
	}

	idt_flush();
	kprintf("IDT successfully loaded!\n");
}

void isr_handler(struct regs *r) {
	if (r->int_no < 32) {
		/* Display the description for the exception that occurred.
		*  For now, simply halt the system using an infinite loop. */
		kputs(exception_msgs[r->int_no]);
		kputs("Exception. System halted");
		for (;;);
	}
}
