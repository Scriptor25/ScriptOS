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

GRUB_CFG = $(SRC_DIR)/grub.cfg
LINKER_LD = $(SRC_DIR)/linker.ld

SRC = $(call rwildcard,$(SRC_DIR),*.s) $(call rwildcard,$(SRC_DIR),*.c) $(call rwildcard,$(SRC_DIR),*.cpp)
OBJ = $(CRT0) $(patsubst $(SRC_DIR)/%,$(BIN_DIR)/%.o,$(SRC))

KERNEL_ELF = $(BIN_DIR)/kernel.elf

OSNAME = scriptos

ISO_DIR = $(BIN_DIR)/iso
ISO = $(BIN_DIR)/$(OSNAME).iso

INCLUDE = -I include
ASFLAGS =
CFLAGS = -ffreestanding -fno-stack-protector -fno-stack-check -fno-pic -mno-red-zone -m64 -Wall -Wextra -Werror
CXXFLAGS = -ffreestanding -fno-stack-protector -fno-stack-check -fno-pic -mno-red-zone -m64 -Wall -Wextra -Werror -fno-exceptions -fno-rtti -std=c++20
LDFLAGS = -nostdlib -static -zmax-page-size=0x1000
QEMU_FLAGS = -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE.fd\
             -drive if=pflash,format=raw,file=/usr/share/OVMF/OVMF_VARS.fd\
			 -cdrom $(ISO)\
			 -m 512\
			 -net none

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
	$(LD) $(LDFLAGS) -o $@ $(OBJ) -T $(LINKER_LD)
	grub-file --is-x86-multiboot2 $@

$(ISO): $(KERNEL_ELF) $(GRUB_CFG)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_ELF) $(ISO_DIR)/boot/kernel.elf
	cp $(GRUB_CFG) $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $@ $(ISO_DIR)
