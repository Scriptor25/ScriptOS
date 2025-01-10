TARGET = i686-elf
AS = $(TARGET)-as
GCC = $(TARGET)-g++
QEMU = qemu-system-i386
CPP = $(TARGET)-cpp

OPT = -O2

GCCFLAGS = -std=c++20 -ffreestanding $(OPT) -Wall -Wextra -fno-exceptions -fno-rtti

SRC = src
BOOTSRC = $(SRC)/boot
KERNELSRC = $(SRC)/kernel
BUILD = build
BOOTBUILD = $(BUILD)/boot
KERNELBUILD = $(BUILD)/kernel

OSNAME = scriptos
KERNEL = $(BUILD)/kernel.bin
ISO = $(BUILD)/$(OSNAME).iso

BOOTOBJS = $(patsubst $(SRC)/%.s,$(BUILD)/%.o,$(wildcard $(BOOTSRC)/*.s))
KERNELOBJS = $(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$(wildcard $(KERNELSRC)/*.cpp))
OBJS = $(BOOTOBJS) $(KERNELOBJS)

.PHONY: build launch clean

build: $(ISO)

launch: $(ISO)
	$(QEMU) -cdrom $(ISO)

debug: $(ISO)
	$(QEMU) -cdrom $(ISO) -s

clean:
	-rm -rf $(BUILD)

$(BOOTBUILD):
	mkdir -p $(BOOTBUILD)

$(KERNELBUILD):
	mkdir -p $(KERNELBUILD)

$(BOOTBUILD)/%.o: $(BOOTBUILD)/%.s.pp
	$(AS) $< -o $@ -I include

$(BOOTBUILD)/%.s.pp: $(BOOTSRC)/%.s $(BOOTBUILD)
	$(CPP) $< -o $@ -I include

$(KERNELBUILD)/%.o: $(KERNELSRC)/%.cpp $(KERNELBUILD)
	$(GCC) -c $< -o $@ $(GCCFLAGS) -I include

$(KERNEL): $(SRC)/linker.ld $(OBJS)
	$(GCC) -T $^ -o $(KERNEL) -ffreestanding $(OPT) -nostdlib -lgcc
	objcopy --only-keep-debug $(KERNEL) $(BUILD)/kernel.sym
	objcopy --strip-debug $(KERNEL)
	grub-file --is-x86-multiboot2 $(KERNEL)

$(ISO): $(KERNEL) $(SRC)/grub.cfg
	mkdir -p $(BUILD)/iso/boot/grub
	cp $(KERNEL) $(BUILD)/iso/boot/kernel.bin
	cp $(SRC)/grub.cfg $(BUILD)/iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(BUILD)/iso
