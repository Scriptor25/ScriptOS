rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

CROSS_COMPILE = x86_64-elf-

AS = $(CROSS_COMPILE)as
CC = $(CROSS_COMPILE)gcc
CXXC = $(CROSS_COMPILE)g++
PP = $(CROSS_COMPILE)cpp
LD = $(CROSS_COMPILE)ld
CPY = $(CROSS_COMPILE)objcopy

QEMU = qemu-system-x86_64

SOURCE_DIRECTORY = src
BINARY_DIRECTORY = bin
LIMINE_DIRECTORY = limine

LIMINE_CONFIG = $(SOURCE_DIRECTORY)/limine.conf
LIMINE_BIOS_UEFI = $(LIMINE_DIRECTORY)/limine-bios.sys $(LIMINE_DIRECTORY)/limine-bios-cd.bin $(LIMINE_DIRECTORY)/limine-uefi-cd.bin
LIMINE_BOOTX64_EFI = $(LIMINE_DIRECTORY)/BOOTX64.EFI

LINKER_LD = $(SOURCE_DIRECTORY)/linker.ld

KERNEL_SOURCES = $(call rwildcard,$(SOURCE_DIRECTORY),*.s) $(call rwildcard,$(SOURCE_DIRECTORY),*.c) $(call rwildcard,$(SOURCE_DIRECTORY),*.cpp)
KERNEL_OBJECTS = $(CRT0) $(KERNEL_SOURCES:$(SOURCE_DIRECTORY)/%=$(BINARY_DIRECTORY)/%.o)
KERNEL_DEPENDECIES = $(KERNEL_OBJECTS:.o=.d)

KERNEL_ELF = $(BINARY_DIRECTORY)/kernel.elf

OS_NAME = scriptos

ISO_DIRECTORY = $(BINARY_DIRECTORY)/iso
ISO = $(BINARY_DIRECTORY)/$(OS_NAME).iso

INCLUDES = -Iinclude -Ilimine -Iefi
DEFINES = -DLIMINE_API_REVISION=3 -DEFI_PLATFORM=1

ASFLAGS = -g -O0
PPFLAGS = $(INCLUDES) $(DEFINES) -MM
CFLAGS = $(INCLUDES) $(DEFINES) -g -g3 -ggdb -O0 -ffreestanding -fno-stack-protector -fno-stack-check -fno-pic -mno-red-zone -m64 -march=x86-64 -mcmodel=kernel -Wall -Wextra -Werror
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti -std=c++20
LDFLAGS = -nostdlib -static

QEMUFLAGS = -cdrom $(ISO) -net none -serial stdio -smp 4 -m 256M -machine q35

.PHONY: all clean build launch-bios debug-bios launch-efi debug-efi

all: clean build launch

clean:
	-rm -rf $(BINARY_DIRECTORY)

build: $(ISO)

launch-bios: $(ISO)
	$(QEMU) $(QEMUFLAGS)

debug-bios: $(ISO)
	$(QEMU) $(QEMUFLAGS) -s -S -no-reboot

launch-efi: $(ISO)
	sudo $(QEMU) $(QEMUFLAGS) -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE.fd -drive if=pflash,format=raw,file=/usr/share/OVMF/OVMF_VARS.fd

debug-efi: $(ISO)
	sudo $(QEMU) $(QEMUFLAGS) -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE.fd -drive if=pflash,format=raw,file=/usr/share/OVMF/OVMF_VARS.fd -s -S -no-reboot

$(BINARY_DIRECTORY)/%.d: $(SOURCE_DIRECTORY)/%
	mkdir -p $(@D)
	-rm $@
	$(PP) $(PPFLAGS) -MT $(BINARY_DIRECTORY)/$*.o -MF $@ $<

-include $(KERNEL_DEPENDECIES)

$(BINARY_DIRECTORY)/%.s.pp: $(SOURCE_DIRECTORY)/%.s
	mkdir -p $(@D)
	-rm $@
	$(PP) $(DEFINES) $(INCLUDES) -o $@ $<

$(BINARY_DIRECTORY)/%.s.o: $(BINARY_DIRECTORY)/%.s.pp
	mkdir -p $(@D)
	-rm $@
	$(AS) $(ASFLAGS) -o $@ $<

$(BINARY_DIRECTORY)/%.c.o: $(SOURCE_DIRECTORY)/%.c
	mkdir -p $(@D)
	-rm $@
	$(CC) $(CFLAGS) -o $@ -c $<

$(BINARY_DIRECTORY)/%.cpp.o: $(SOURCE_DIRECTORY)/%.cpp
	mkdir -p $(@D)
	-rm $@
	$(CXXC) $(CXXFLAGS) -o $@ -c $<

$(BINARY_DIRECTORY)/interrupt/%.c.o: $(SOURCE_DIRECTORY)/interrupt/%.c
	mkdir -p $(@D)
	-rm $@
	$(CC) $(CFLAGS) -mgeneral-regs-only -o $@ -c $<

$(BINARY_DIRECTORY)/interrupt/%.cpp.o: $(SOURCE_DIRECTORY)/interrupt/%.cpp
	mkdir -p $(@D)
	-rm $@
	$(CXXC) $(CXXFLAGS) -mgeneral-regs-only -o $@ -c $<

$(KERNEL_ELF): $(KERNEL_OBJECTS) $(LINKER_LD)
	mkdir -p $(@D)
	-rm $@
	$(LD) $(LDFLAGS) -o $(KERNEL_ELF) $(KERNEL_OBJECTS) -T $(LINKER_LD)


$(ISO): $(KERNEL_ELF) $(LIMINE_CONFIG) $(LIMINE_BIOS_UEFI) $(LIMINE_BOOTX64_EFI)
	-rm -rf $(ISO_DIRECTORY)
	mkdir -p $(ISO_DIRECTORY)/boot
	cp $(KERNEL_ELF) $(ISO_DIRECTORY)/boot/kernel.elf
	mkdir -p $(ISO_DIRECTORY)/boot/limine
	cp $(LIMINE_CONFIG) $(LIMINE_BIOS_UEFI) $(ISO_DIRECTORY)/boot/limine
	mkdir -p $(ISO_DIRECTORY)/EFI/BOOT
	cp $(LIMINE_BOOTX64_EFI) $(ISO_DIRECTORY)/EFI/BOOT/BOOTX64.EFI
	mkdir -p $(@D)
	-rm $@
	xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus -apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin -efi-boot-part --efi-boot-image --protective-msdos-label $(ISO_DIRECTORY) -o $(ISO)
