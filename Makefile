rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

TARGET = i686-elf
AS = $(TARGET)-as
CC = $(TARGET)-g++
PP = $(TARGET)-cpp
QEMU = qemu-system-i386

OPT = -O0 -g -ggdb -g3
CFLAGS = $(OPT) -std=c++20 -ffreestanding -mno-red-zone -Wall -Wextra -fno-exceptions -fno-rtti
LDFLAGS = $(OPT) -ffreestanding -nostdlib -lgcc

SRC_DIR = src
BIN_DIR = bin

INCLUDE = -I include

KERNEL_SRC_DIR = $(SRC_DIR)/kernel
KERNEL_BIN = $(BIN_DIR)/kernel.elf
KERNEL_SYM = $(BIN_DIR)/kernel.sym
KERNEL_ASM_SRC = $(call rwildcard,$(KERNEL_SRC_DIR),*.s)
KERNEL_CPP_SRC = $(call rwildcard,$(KERNEL_SRC_DIR),*.cpp)
KERNEL_ASM_OBJ = $(patsubst $(SRC_DIR)/%.s,$(BIN_DIR)/%.s.o,$(KERNEL_ASM_SRC))
KERNEL_CPP_OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.cpp.o,$(KERNEL_CPP_SRC))
KERNEL_OBJ = $(KERNEL_ASM_OBJ) $(KERNEL_CPP_OBJ)

USER_SRC_DIR = $(SRC_DIR)/user
USER_BIN = $(BIN_DIR)/user.elf
USER_ASM_SRC = $(call rwildcard,$(USER_SRC_DIR),*.s)
USER_CPP_SRC = $(call rwildcard,$(USER_SRC_DIR),*.cpp)
USER_ASM_OBJ = $(patsubst $(SRC_DIR)/%.s,$(BIN_DIR)/%.s.o,$(USER_ASM_SRC))
USER_CPP_OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.cpp.o,$(USER_CPP_SRC))
USER_OBJ = $(USER_ASM_OBJ) $(USER_CPP_OBJ)

OSNAME = scriptos

ISO_DIR = $(BIN_DIR)/iso
ISO = $(BIN_DIR)/$(OSNAME).iso

.PHONY: all clean build launch debug

all: clean build launch

clean:
	-rm -rf $(BIN_DIR)

build: $(ISO)

launch: $(ISO)
	$(QEMU) -cdrom $(ISO)

debug: $(ISO)
	$(QEMU) -cdrom $(ISO) -s

$(BIN_DIR)/%.s.pp: $(SRC_DIR)/%.s
	mkdir -p $(@D)
	$(PP) -o $@ $(INCLUDE) $<

$(BIN_DIR)/%.s.o: $(BIN_DIR)/%.s.pp
	mkdir -p $(@D)
	$(AS) -o $@ $<

$(BIN_DIR)/kernel/interrupts.cpp.o: $(SRC_DIR)/kernel/interrupts.cpp
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -mgeneral-regs-only -o $@ $(INCLUDE) -c $<

$(BIN_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $(INCLUDE) -c $<

$(KERNEL_BIN): $(KERNEL_SRC_DIR)/linker.ld $(KERNEL_OBJ)
	$(CC) $(LDFLAGS) -o $@ -T $^
	objcopy --only-keep-debug $(KERNEL_BIN) $(KERNEL_SYM)
	objcopy --strip-debug $(KERNEL_BIN)
	grub-file --is-x86-multiboot2 $(KERNEL_BIN)

$(USER_BIN): $(USER_SRC_DIR)/linker.ld $(USER_OBJ)
	$(CC) $(LDFLAGS) -o $@ -T $^

$(ISO): $(KERNEL_BIN) $(KERNEL_SRC_DIR)/grub.cfg
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_BIN) $(ISO_DIR)/boot/kernel.elf
	cp $(KERNEL_SRC_DIR)/grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(ISO_DIR)
