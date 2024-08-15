# Define directories
SRC_DIR = src
BUILD_DIR = build
CONFIG_DIR = cfg
INCLUDE_DIR = include/internal
ISO_DIR = $(BUILD_DIR)/isodir

# Define file names
BOOT_ASM = $(SRC_DIR)/boot.asm
LINKER_SCRIPT = $(SRC_DIR)/linker.ld
BIN_FILE = $(BUILD_DIR)/myos.bin
ISO_FILE = $(BUILD_DIR)/myos.iso
GRUB_CFG = $(CONFIG_DIR)/grub.cfg

# Define tools
NASM = nasm
GCC = i686-elf-gcc
GRUB_FILE = grub-file
GRUB_MKRESCUE = grub-mkrescue

# Define compiler flags
CFLAGS = -I$(INCLUDE_DIR) -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -ffreestanding -O2 -nostdlib -lgcc

# Define source files and headers
KERNEL_SRCS = $(wildcard $(SRC_DIR)/*.c)
KERNEL_OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(KERNEL_SRCS))
KERNEL_HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)
BOOT_OBJ = $(BUILD_DIR)/boot.o

# Targets and rules
all: $(ISO_FILE)

# Ensure the build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Ensure the ISO directory exists
$(ISO_DIR): $(BUILD_DIR)
	mkdir -p $(ISO_DIR)/boot/grub

# Rule for building the final ISO file
$(ISO_FILE): $(BIN_FILE) $(ISO_DIR) check-multiboot
	cp $(BIN_FILE) $(ISO_DIR)/boot/myos.bin
	cp $(GRUB_CFG) $(ISO_DIR)/boot/grub/grub.cfg
	$(GRUB_MKRESCUE) -o $(ISO_FILE) $(ISO_DIR)

# Rule for linking the final binary
$(BIN_FILE): $(BOOT_OBJ) $(KERNEL_OBJS) $(BUILD_DIR)
	$(GCC) -T $(LINKER_SCRIPT) -o $(BIN_FILE) $(LDFLAGS) $(BOOT_OBJ) $(KERNEL_OBJS)

# Rule for compiling the kernel object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(KERNEL_HEADERS) $(BUILD_DIR)
	$(GCC) -c $< -o $@ $(CFLAGS)

# Rule for assembling the boot object file
$(BOOT_OBJ): $(BOOT_ASM) $(BUILD_DIR)
	$(NASM) -f elf32 $(BOOT_ASM) -o $(BOOT_OBJ)

# Check if the binary file conforms to the multiboot standard
check-multiboot: $(BIN_FILE)
	@echo "checking multiboot.."
	@if grub-file --is-x86-multiboot $(BIN_FILE) > /dev/null 2>&1; then \
		echo "multiboot confirmed"; \
	else \
		echo "the file is not multiboot"; \
		exit 1; \
	fi

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all clean check-multiboot

