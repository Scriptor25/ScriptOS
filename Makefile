AS = i686-elf-as
GCC = i686-elf-gcc
QEMU = qemu-system-i386
CPP = i686-elf-cpp

GCCFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra

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
KERNELOBJS = $(patsubst $(SRC)/%.c,$(BUILD)/%.o,$(wildcard $(KERNELSRC)/*.c))
OBJS = $(BOOTOBJS) $(KERNELOBJS)

.PHONY: build launch clean

build: $(ISO)

launch: $(ISO)
	$(QEMU) -cdrom $(ISO)

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

$(KERNELBUILD)/%.o: $(KERNELSRC)/%.c $(KERNELBUILD)
	$(GCC) -c $< -o $@ $(GCCFLAGS) -I include

$(KERNEL): $(SRC)/linker.ld $(OBJS)
	$(GCC) -o $(KERNEL) -ffreestanding -O2 -nostdlib -T $^ -lgcc
	grub-file --is-x86-multiboot2 $(KERNEL)

$(ISO): $(KERNEL) $(SRC)/grub.cfg
	mkdir -p $(BUILD)/iso/boot/grub
	cp $(KERNEL) $(BUILD)/iso/boot/kernel.bin
	cp $(SRC)/grub.cfg $(BUILD)/iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(BUILD)/iso
