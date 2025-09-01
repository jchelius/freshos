#pragma once
#include <stdint.h>

void pic_send_eoi(uint8_t irq);
void pic_remap(uint8_t offset1, uint8_t offset2);
void pic_disable(void);
int pic_set_mask(uint8_t irq_line);
int pic_clear_mask(uint8_t irq_line);
uint16_t pic_get_irr(void);
uint16_t pic_get_isr(void);
