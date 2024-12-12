#pragma once
#include <stdint.h>

int kprintf(const char* __restrict, ...);
int kprintf_color(uint8_t color, const char* __restrict, ...);
