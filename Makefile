rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

CROSS_COMPILE = x86_64-w64-mingw32-

AS = $(CROSS_COMPILE)as
CC = $(CROSS_COMPILE)gcc
CXXC = $(CROSS_COMPILE)g++
PP = $(CROSS_COMPILE)cpp
LD = $(CROSS_COMPILE)ld
CPY = objcopy

QEMU = qemu-system-x86_64

GRUB_CFG = grub.cfg

GNU_EFI_DIR = gnu-efi

INCLUDE = -I include -I $(GNU_EFI_DIR)/inc
ASFLAGS =
CFLAGS = -fpic -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar -mno-red-zone -Wall -Wextra -Werror
CXXFLAGS = -fpic -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar -mno-red-zone -Wall -Wextra -Werror -fno-exceptions -fno-rtti -std=c++20
LDFLAGS = -shared -Bsymbolic -nostdlib -L $(GNU_EFI_DIR)/x86_64/lib -L $(GNU_EFI_DIR)/x86_64/gnuefi -T $(GNU_EFI_DIR)/gnuefi/elf_x86_64_efi.lds
CPYFLAGS = -j .text -j .sdata -j .data -j .rodata -j .dynamic -j dynsym -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc --output-target=efi-app-x86_64 --subsystem=10

SRC_DIR = src
BIN_DIR = bin

CRT0 = $(GNU_EFI_DIR)/x86_64/gnuefi/crt0-efi-x86_64.o

SRC = $(call rwildcard,$(SRC_DIR),*.s) $(call rwildcard,$(SRC_DIR),*.c) $(call rwildcard,$(SRC_DIR),*.cpp)
OBJ = $(CRT0) $(patsubst $(SRC_DIR)/%,$(BIN_DIR)/%.o,$(SRC))

TARGET_SO = $(BIN_DIR)/kernel.so
TARGET_EFI = $(BIN_DIR)/kernel.efi

OSNAME = scriptos

ISO_DIR = $(BIN_DIR)/iso
ISO = $(BIN_DIR)/$(OSNAME).iso

QEMU_FLAGS = -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE.fd -drive if=pflash,format=raw,file=bin/OVMF_VARS.fd -cdrom $(ISO) -m 512

.PHONY: all clean build launch debug gnuefi

all: clean build launch

clean:
	-rm -rf $(BIN_DIR)

build: $(ISO)

launch: $(ISO)
	$(QEMU) $(QEMU_FLAGS)

debug: $(ISO)
	$(QEMU) $(QEMU_FLAGS) -s

gnuefi:
	$(MAKE) -C $(GNU_EFI_DIR) CROSS_COMPILE=$(CROSS_COMPILE) clean gnuefi lib

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

$(TARGET_SO): $(OBJ)
	mkdir -p $(@D)
	$(LD) $(LDFLAGS) -o $@ $^ -lgnuefi -lefi

$(TARGET_EFI): $(TARGET_SO)
	mkdir -p $(@D)
	$(CPY) $(CPYFLAGS) $< $@

$(ISO): $(TARGET_EFI)
	mkdir -p $(ISO_DIR)/EFI/BOOT
	mkdir -p $(ISO_DIR)/boot/grub
	cp $< $(ISO_DIR)/EFI/BOOT/BOOTX64.EFI
	cp $(GRUB_CFG) $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $@ $(ISO_DIR)
