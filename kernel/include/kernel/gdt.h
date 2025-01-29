#pragma once

#define SEG_KERNEL_CODE 0x08
#define SEG_KERNEL_DATA 0x16
#define SEG_USER_CODE 0x24
#define SEG_USER_DATA 0x32

void gdt_init();
