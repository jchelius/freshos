#include "system.h"

#define PIC1		0x20	/* IO Base address for master PIC */
#define PIC2		0xA0	/* IO Base address for slave PIC */
#define PIC1_CMD	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_CMD	PIC2
#define PIC2_DATA	(PIC2+1)

#define PIC_EOI		0x20	/* End-of-interrupt command code */

#define ICW1_ICW4	0x01	/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	0x02	/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04 /* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08	/* Level triggered (edge) mode */
#define ICW1_INIT	0x10	/* Initialization */

#define ICW4_8086	0x01	/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02	/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08	/* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C	/* Buffered mode/master */
#define ICW4_SFNM	0x10	/* Special fully nested (not) */

// #define CASCADE_IRQ 2

#define PIC_READ_IRR 0x0A /* OCW3 irq ready next CMD read */
#define PIC_READ_ISR 0x0B /* OCW3 irq service next CMD read */

void pic_send_eoi(uint8_t irq) {
	// Send EOI to PIC2 if IRQ came from PIC2
	if (irq >= 8) {
		outportb(PIC2_CMD, PIC_EOI);
	}

	// Always send EOI command to PIC1
	outportb(PIC1_CMD, PIC_EOI);
}

static void io_wait(void) {
	// Port 0x80 is unused; hence writing 0 to this port will
	// result in a small delay.
	outportb(0x80, 0);
}

/* 
arguments:
	offset1 - vector offset for master PIC
		vectors on the master become offset1..offset1+7
	offset2 - same for slave PIC: offset2..offset2+7
*/
void pic_remap(uint8_t offset1, uint8_t offset2) {
	// ICW1: Start initialization sequence in cascade mode
	outportb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);
	// On older machines, it is necessary to delay for a bit before proceeding
	io_wait();
	outportb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);
	io_wait();
	// ICW2: Master PIC vector offset
	outportb(PIC1_DATA, offset1);
	io_wait();
	// ICW2: Slave PIC vector offset
	outportb(PIC2_DATA, offset2);
	io_wait();
	// ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	outportb(PIC1_DATA, 0x04);
	io_wait();
	// ICW3: tell Slave PIC its cascade identity (0000 0010)
	outportb(PIC2_DATA, 2);
	io_wait();

	// ICW4: have the PICS use 8086 mode (and not 8080 mode)
	outportb(PIC1_DATA, ICW4_8086);
	io_wait();
	outportb(PIC2_DATA, ICW4_8086);
	io_wait();

	// Unmask both PICS:
	outportb(PIC1_DATA, 0);
	outportb(PIC2_DATA, 0);
}

void pic_disable(void) {
	outportb(PIC1_DATA, 0xFF);
	outportb(PIC2_DATA, 0xFF);
}

static int pic_set_or_clear_mask(uint8_t irq_line, int set) {
	// Set the Interrupt Mask Register (IMR) to mask interrupt request.
	// Returns 0 on success; -1 on failure.
	uint16_t port;
	uint8_t val;

	if (irq_line < 8) {
		port = PIC1_DATA;
	} else if (irq_line < 16) {
		port = PIC2_DATA;
		irq_line -= 8;
	} else {
		return -1;
	}
	if (set) {
		val = inportb(port) | (uint8_t) (1 << irq_line);
	} else {
		val = inportb(port) & ~(1 << irq_line);
	}
	outportb(port, val);
	return 0;
}

int pic_set_mask(uint8_t irq_line) {
	// Set the Interrupt Mask Register (IMR) to mask interrupt request.
	// Returns 0 on success; -1 on failure.
	return pic_set_or_clear_mask(irq_line, 1);
}

int pic_clear_mask(uint8_t irq_line) {
	// Clear the Interrupt Mask Register (IMR) to mask interrupt request.
	// Returns 0 on success; -1 on failure.
	return pic_set_or_clear_mask(irq_line, 0);
}

static uint16_t pic_get_irq_reg(uint8_t ocw3) {
	/* OCW3 to PIC CMD to get the register values. PIC2 is chained, and 
	 * represents IRQs 8-15. PIC1 is IRQs 0-7, with 2 being the chain. */
	outportb(PIC1_CMD, ocw3);
	outportb(PIC2_CMD, ocw3);

	return ((uint16_t) (inportb(PIC2_CMD) << 8)) | inportb(PIC1_CMD);
}

uint16_t pic_get_irr(void) {
	return pic_get_irq_reg(PIC_READ_IRR);
}

uint16_t pic_get_isr(void) {
	return pic_get_irq_reg(PIC_READ_ISR);
}

