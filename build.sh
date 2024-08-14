SRC_DIR=src
BUILD_DIR=build
CONFIG_DIR=cfg

mkdir -p ${BUILD_DIR}

nasm -felf32 ${SRC_DIR}/boot.asm -o ${BUILD_DIR}/boot.o
i686-elf-gcc -c ${SRC_DIR}/kernel.c -o ${BUILD_DIR}/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -T ${SRC_DIR}/linker.ld -o ${BUILD_DIR}/myos.bin -ffreestanding -O2 -nostdlib ${BUILD_DIR}/boot.o ${BUILD_DIR}/kernel.o -lgcc

# verify that file conforms to multiboot standard
if grub-file --is-x86-multiboot ${BUILD_DIR}/myos.bin; then
    echo multiboot confirmed
else
    echo the file is not multiboot
    return 1
fi

mkdir -p ${BUILD_DIR}/isodir/boot/grub
cp ${BUILD_DIR}/myos.bin ${BUILD_DIR}/isodir/boot/myos.bin
cp ${CONFIG_DIR}/grub.cfg ${BUILD_DIR}/isodir/boot/grub/grub.cfg
grub-mkrescue -o ${BUILD_DIR}/myos.iso ${BUILD_DIR}/isodir
