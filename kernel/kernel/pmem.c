#include <stdint.h>

#define MAX_FRAMES (1024*1024*1024 / 4096) // 1 GB
#define BITMAP_SIZE (MAX_FRAMES + 7) / 8 // Integer hack to round up

static uint8_t physical_bitmap[BITMAP_SIZE];

uintptr_t pmem_alloc_frame() {

}
