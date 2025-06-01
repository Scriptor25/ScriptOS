rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

CROSS_COMPILE = x86_64-elf-

AS = $(CROSS_COMPILE)as
CC = $(CROSS_COMPILE)gcc
CXXC = $(CROSS_COMPILE)g++
PP = $(CROSS_COMPILE)cpp
LD = $(CROSS_COMPILE)ld
CPY = $(CROSS_COMPILE)objcopy

QEMU = qemu-system-x86_64

SRC_DIR = src
BIN_DIR = bin

LIMINE_CONF = $(SRC_DIR)/limine.conf
LIMINE_BIOS_UEFI = limine/limine-bios.sys limine/limine-bios-cd.bin limine/limine-uefi-cd.bin
LIMINE_BOOTX64 = limine/BOOTX64.EFI

LINKER_LD = $(SRC_DIR)/linker.ld

SRC = $(call rwildcard,$(SRC_DIR),*.s) $(call rwildcard,$(SRC_DIR),*.c) $(call rwildcard,$(SRC_DIR),*.cpp)
OBJ = $(CRT0) $(patsubst $(SRC_DIR)/%,$(BIN_DIR)/%.o,$(SRC))

KERNEL_ELF = $(BIN_DIR)/kernel.elf

OSNAME = scriptos

ISO_DIR = $(BIN_DIR)/iso
ISO = $(BIN_DIR)/$(OSNAME).iso

INCLUDE = -I include
OPT = -g -g3 -ggdb -O0

ASFLAGS = -g -O0
CFLAGS = $(OPT) -ffreestanding -fno-stack-protector -fno-stack-check -fno-pic -mno-red-zone -m64 -Wall -Wextra -Werror
CXXFLAGS = $(OPT) -ffreestanding -fno-stack-protector -fno-stack-check -fno-pic -mno-red-zone -m64 -Wall -Wextra -Werror -fno-exceptions -fno-rtti -std=c++20
LDFLAGS = -nostdlib -static
QEMU_FLAGS = -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE.fd -drive if=pflash,format=raw,file=/usr/share/OVMF/OVMF_VARS.fd -cdrom $(ISO) -m 512 -net none

.PHONY: all clean build launch debug

all: clean build launch

clean:
	-rm -rf $(BIN_DIR)

build: $(ISO)

launch: $(ISO)
	sudo $(QEMU) $(QEMU_FLAGS)

debug: $(ISO)
	sudo $(QEMU) $(QEMU_FLAGS) -s

$(BIN_DIR)/%.s.pp: $(SRC_DIR)/%.s
	mkdir -p $(@D)
	$(PP) $(INCLUDE) -o $@ $<

$(BIN_DIR)/%.s.o: $(BIN_DIR)/%.s.pp
	mkdir -p $(@D)
	$(AS) $(ASFLAGS) -o $@ $<

$(BIN_DIR)/%.c.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(INCLUDE) $(CFLAGS) -o $@ -c $<

$(BIN_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CXXC) $(INCLUDE) $(CXXFLAGS) -o $@ -c $<

$(KERNEL_ELF): $(OBJ) $(LINKER_LD)
	mkdir -p $(@D)
	$(LD) $(LDFLAGS) -o $(KERNEL_ELF) $(OBJ) -T $(LINKER_LD)
	grub-file --is-x86-multiboot2 $(KERNEL_ELF)

$(ISO): $(KERNEL_ELF) $(LIMINE_CONF) $(LIMINE_BIOS_UEFI) $(LIMINE_BOOTX64)
	mkdir -p $(ISO_DIR)/boot
	cp $(KERNEL_ELF) $(ISO_DIR)/boot/kernel.elf

	mkdir -p $(ISO_DIR)/boot/limine
	cp $(LIMINE_CONF) $(LIMINE_BIOS_UEFI) $(ISO_DIR)/boot/limine/

	mkdir -p $(ISO_DIR)/EFI/BOOT
	cp $(LIMINE_BOOTX64) $(ISO_DIR)/EFI/BOOT

	xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus -apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin -efi-boot-part --efi-boot-image --protective-msdos-label $(ISO_DIR) -o $@
