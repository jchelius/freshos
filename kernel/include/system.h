#pragma once
#include <stdint.h>

static inline uint8_t inportb(uint16_t port) {
    uint8_t rv;
    __asm__ volatile("inb %w1, %b0" : "=a"(rv) : "dN"(port) : "memory");
    return rv;
}

static inline void outportb(uint16_t port, uint8_t data) {
    __asm__ volatile("outb %b0, %w1" : : "a"(data), "dN"(port) : "memory");
}
