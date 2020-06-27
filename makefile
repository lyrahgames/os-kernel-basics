AS = ~/opt/cross/bin/i686-elf-as

CC = ~/opt/cross/bin/i686-elf-gcc
CC_FLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
CC_LD_FLAGS = -ffreestanding -O2 -nostdlib
CC_LIBS = -lgcc

CXX= ~/opt/cross/bin/i686-elf-g++
CXX_INCLUDE_DIRS = -I.
CXX_FLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -std=c++20

ISODIR = isodir

.PHONY: all clean test run run_kernel

all: myos.iso

boot.o: boot.s
	$(AS) boot.s -o boot.o

# kernel.o: kernel.c
# 	$(CC) -c kernel.c -o kernel.o $(CC_FLAGS)
kernel.o: vga.hpp kernel.cpp
	$(CXX) $(CXX_INCLUDE_DIRS) -c kernel.cpp -o kernel.o $(CXX_FLAGS)

myos.bin: boot.o kernel.o
	$(CC) -T linker.ld -o myos.bin $(CC_LD_FLAGS) boot.o kernel.o $(CC_LIBS)

myos.iso: myos.bin grub.cfg
	mkdir -p $(ISODIR)/boot/grub
	cp myos.bin $(ISODIR)/boot/myos.bin
	cp grub.cfg $(ISODIR)/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso $(ISODIR)

clean:
	rm -f boot.o
	rm -f kernel.o
	rm -f myos.bin
	rm -rf $(ISODIR)
	rm -f myos.iso

test: myos.bin
	if grub-file --is-x86-multiboot myos.bin; then \
		echo "Multiboot confirmed."; \
	else \
		echo "Could not verify multiboot."; \
	fi

run: myos.iso
	qemu-system-i386 -cdrom myos.iso

run_kernel: myos.bin
	qemu-system-i386 -kernel myos.bin