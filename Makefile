# Makefile for MiniOS
CC = gcc
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -nostdlib -fno-builtin -fno-pic -fno-pie -fno-stack-protector
AS = nasm
ASFLAGS = -f elf32
LD = ld
LDFLAGS = -m elf_i386 -T linker.ld

BUILD = build
ISO_DIR = iso
GRUB_DIR = $(ISO_DIR)/boot/grub
ISO_FILE = minios.iso

OBJS = $(BUILD)/boot.o $(BUILD)/kernel.o $(BUILD)/shell.o $(BUILD)/scheduler.o $(BUILD)/memory.o

all: $(ISO_FILE)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/boot.o: asm/boot.asm | $(BUILD)
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD)/kernel.o: src/kernel.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/shell.o: src/shell.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/scheduler.o: src/scheduler.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/memory.o: src/memory.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

$(ISO_FILE): $(BUILD)/kernel.bin grub/grub.cfg
	mkdir -p $(GRUB_DIR)
	cp $(BUILD)/kernel.bin $(ISO_DIR)/boot/kernel.bin
	cp grub/grub.cfg $(GRUB_DIR)/grub.cfg
	grub-mkrescue -o $(ISO_FILE) $(ISO_DIR)
	@echo "===================================="
	@echo "ISO created: $(ISO_FILE)"
	@echo "Load this file in VMware to test!"
	@echo "===================================="

clean:
	rm -rf $(BUILD) $(ISO_FILE) $(ISO_DIR)/boot/kernel.bin

.PHONY: all clean
