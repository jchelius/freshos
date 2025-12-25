#pragma once
#include <stdint.h>

#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

// No packing required: Multiboot layout matches natural C alignment
struct multiboot_info {
	/* flags */
	uint32_t flags;                   // 0-3

	/* mem */
	uint32_t mem_lower;               // 4-7
	uint32_t mem_upper;               // 8-11

	/* boot_device */
	uint32_t boot_device;             // 12-15

	/* cmdline */
	uint32_t cmdline;                 // 16-19

	/* mods */
	uint32_t mods_count;              // 20-23
	uint32_t mods_addr;               // 24-27

	/* syms */
	union {
		struct {
			uint32_t tabsize;
			uint32_t strsize;
			uint32_t addr;
			uint32_t reserved;
		} aout_sym;

		struct {
			uint32_t num;
			uint32_t size;
			uint32_t addr;
			uint32_t shndx;
		} elf_sec;
	} syms;                           // 28-43

	/* mmap */
	uint32_t mmap_length;             // 44-47
	uint32_t mmap_addr;               // 48-51

	/* drives */
	uint32_t drives_length;           // 52-55
	uint32_t drives_addr;             // 56-59

	/* config */
	uint32_t config_table;            // 60-63

	/* boot_loader */
	uint32_t boot_loader_name;        // 64-67

	/* apm */
	uint32_t apm_table;               // 68-71

	/* vbe */
	uint32_t vbe_control_info;        // 72-75
	uint32_t vbe_mode_info;           // 76-79
	uint16_t vbe_mode;                // 80-81
	uint16_t vbe_interface_seg;       // 82-83
	uint16_t vbe_interface_off;       // 84-85
	uint16_t vbe_interface_len;       // 86-87

	/* framebuffer */
	uint64_t framebuffer_addr;        // 88-95
	uint32_t framebuffer_pitch;       // 96-99
	uint32_t framebuffer_width;       // 100-103
	uint32_t framebuffer_height;      // 104-107
	uint8_t framebuffer_bpp;          // 108-108
	uint8_t framebuffer_type;         // 109-109
	uint8_t color_info2[6];           // 110-115
};

// struct __attribute__((packed)) multiboot_mmem_info {
struct multiboot_mmap_entry {
	uint32_t size; // -4 -3 -2 -1; 0 1 2 3
	uint64_t base_addr; // 0 1 2 3 4 5 6 7; 4 5 6 7 8 9 10 11
	uint64_t length; // 8 9 10 11 12 13 14 15; 12 13 14 15 16 17 18 19
	uint32_t type; // 16; 20
};
