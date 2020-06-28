AS = ~/opt/cross/bin/i686-elf-as

CC = ~/opt/cross/bin/i686-elf-gcc
CC_FLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
CC_LD_FLAGS = -ffreestanding -O2 -nostdlib
CC_LIBS = -lgcc

CXX= ~/opt/cross/bin/i686-elf-g++
CXX_INCLUDE_DIRS = -I.
CXX_FLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -std=c++20

OBJS:=boot.o kernel.o icxxabi.o

CRTI_OBJ=crti.o
CRTBEGIN_OBJ:=$(shell $(CXX) $(CXX_FLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CXX) $(CXX_FLAGS) -print-file-name=crtend.o)
CRTN_OBJ=crtn.o

OBJ_LINK_LIST:=$(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OBJS) $(CRTEND_OBJ) $(CRTN_OBJ)
INTERNAL_OBJS:=$(CRTI_OBJ) $(OBJS) $(CRTN_OBJ)

TARGET:=myos.bin
TARGET_ISO:=myos.iso

ISODIR = isodir

$(info $(CRTBEGIN_OBJ))
$(info $(CRTEND_OBJ))

.PHONY: all clean test run run_kernel

all: myos.iso

boot.o: boot.s
	$(AS) boot.s -o boot.o

icxxabi.o: icxxabi.cpp icxxabi.hpp
	$(CXX) $(CXX_INCLUDE_DIRS) -c icxxabi.cpp -o icxxabi.o $(CXX_FLAGS)

kernel.o: icxxabi.hpp vga.hpp kernel.cpp
	$(CXX) $(CXX_INCLUDE_DIRS) -c kernel.cpp -o kernel.o $(CXX_FLAGS)

$(TARGET): $(OBJ_LINK_LIST) linker.ld
	$(CC) -T linker.ld -o $(TARGET) $(CC_LD_FLAGS) $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OBJS) $(CC_LIBS) $(CRTEND_OBJ) $(CRTN_OBJ)

$(TARGET_ISO): $(TARGET) grub.cfg
	mkdir -p $(ISODIR)/boot/grub
	cp $(TARGET) $(ISODIR)/boot/$(TARGET)
	cp grub.cfg $(ISODIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(TARGET_ISO) $(ISODIR)

clean:
	rm -f $(TARGET) $(INTERNAL_OBJS)
	rm -rf $(ISODIR)
	rm -f $(TARGET_ISO)

test: $(TARGET)
	if grub-file --is-x86-multiboot $(TARGET); then \
		echo "Multiboot confirmed."; \
	else \
		echo "Could not verify multiboot."; \
	fi

run: $(TARGET_ISO)
	qemu-system-i386 -cdrom $(TARGET_ISO)

run_kernel: $(TARGET)
	qemu-system-i386 -kernel $(TARGET)