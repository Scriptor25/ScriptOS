TARGET = i686-elf
AS = $(TARGET)-as
CC = $(TARGET)-g++
PP = $(TARGET)-cpp
QEMU = qemu-system-i386

OPT = -O2
CCFLAGS = -std=c++20 -ffreestanding $(OPT) -Wall -Wextra -fno-exceptions -fno-rtti
LDFLAGS = -ffreestanding $(OPT) -nostdlib -lgcc

SRC_DIR = src
BIN_DIR = bin

OSNAME = scriptos
KERNEL_BIN = $(BIN_DIR)/kernel.bin
KERNEL_SYM = $(BIN_DIR)/kernel.sym

ISO_DIR = $(BIN_DIR)/iso
ISO = $(BIN_DIR)/$(OSNAME).iso

rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

ASM_SRC = $(call rwildcard,$(SRC_DIR),*.s)
CPP_SRC = $(call rwildcard,$(SRC_DIR),*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.s,$(BIN_DIR)/%.s.o,$(ASM_SRC)) $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.cpp.o,$(CPP_SRC))

.PHONY: build launch clean

build: $(ISO)

launch: $(ISO)
	$(QEMU) -cdrom $(ISO)

debug: $(ISO)
	$(QEMU) -cdrom $(ISO) -s

clean:
	-rm -rf $(BIN_DIR)

$(BIN_DIR)/%.s.o: $(BIN_DIR)/%.s.pp
	mkdir -p $(@D)
	$(AS) $< -o $@

$(BIN_DIR)/%.s.pp: $(SRC_DIR)/%.s
	mkdir -p $(@D)
	$(PP) $< -o $@ -I include

$(BIN_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CCFLAGS) -I include

$(KERNEL_BIN): $(SRC_DIR)/linker.ld $(OBJS)
	$(CC) -T $^ -o $(KERNEL_BIN) $(LDFLAGS)
	objcopy --only-keep-debug $(KERNEL_BIN) $(KERNEL_SYM)
	objcopy --strip-debug $(KERNEL_BIN)
	grub-file --is-x86-multiboot2 $(KERNEL_BIN)

$(ISO): $(KERNEL_BIN) $(SRC_DIR)/grub.cfg
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_BIN) $(ISO_DIR)/boot/kernel.bin
	cp $(SRC_DIR)/grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(ISO_DIR)
