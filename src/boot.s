#define ASM_FILE 1
#include <scriptos/multiboot2.h>

.set MAGIC,         MULTIBOOT2_HEADER_MAGIC
.set ARCHITECTURE,  MULTIBOOT_ARCHITECTURE_I386
.set HEADER_LENGTH, multiboot_header_end - multiboot_header
.set CHECKSUM,      -(MAGIC + ARCHITECTURE + HEADER_LENGTH)
.set STACK_SIZE,    0x4000

.section .multiboot
    .align MULTIBOOT_TAG_ALIGN
multiboot_header:
    # HEADER
    .long MAGIC
    .long ARCHITECTURE
    .long HEADER_LENGTH
    .long CHECKSUM

    # ADDRESS TAG
    .align MULTIBOOT_TAG_ALIGN
    .short MULTIBOOT_HEADER_TAG_ADDRESS
    .short 0
    .long 24
    .long multiboot_header
    .long __text
    .long __data_end
    .long __bss_end

    # ENTRY ADDRESS
    .align MULTIBOOT_TAG_ALIGN
    .short MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS
    .short 0
    .long 12
    .long multiboot_entry

    # EFI64 ENTRY ADDRESS
    .align MULTIBOOT_TAG_ALIGN
    .short MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS_EFI64
    .short 0
    .long 12
    .long multiboot_entry

    # FRAMEBUFFER
    .align MULTIBOOT_TAG_ALIGN
    .short MULTIBOOT_HEADER_TAG_FRAMEBUFFER
    .short MULTIBOOT_HEADER_TAG_OPTIONAL
    .long 20
    .long 1024
    .long 768
    .long 32

    # MODULE ALIGNMENT
    .align MULTIBOOT_TAG_ALIGN
    .short MULTIBOOT_HEADER_TAG_MODULE_ALIGN
    .short 0
    .long 8

    # END TAG
    .align MULTIBOOT_TAG_ALIGN
    .short MULTIBOOT_HEADER_TAG_END
    .short 0
    .long 8
multiboot_header_end:

.section .text
.global multiboot_entry
.type multiboot_entry, @function
multiboot_entry:
    # SETUP STACK
    mov $stack_top, %esp

    # CLEAR FLAGS
    push $0
    popf

    # SETUP START ARGUMENTS
    push %rbx # BOOT INFO STRUCT
    push %rax # MAGIC

    # JUMP TO MAIN
    call kernel_main

.halt: # HALT SYSTEM ON STOP
    cli
    hlt
    jmp .halt

.section .bss
.align 8
.skip STACK_SIZE
stack_top:
