#define ASM_FILE 1
#include <scriptos/multiboot2.h>

.set MAGIC,         MULTIBOOT2_HEADER_MAGIC
.set ARCHITECTURE,  MULTIBOOT_ARCHITECTURE_I386
.set HEADER_LENGTH, multiboot_header_end - multiboot_header
.set CHECKSUM,      -(MAGIC + ARCHITECTURE + HEADER_LENGTH)
.set STACK_SIZE,    0x4000

.section .multiboot
    # HEADER
    .align MULTIBOOT_HEADER_ALIGN
multiboot_header:
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
    .long __load
    .long __load_end
    .long __bss_end

    # ENTRY ADDRESS
    .align MULTIBOOT_TAG_ALIGN
    .short MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS
    .short 0
    .long 12
    .long _start

    # EFI64 ENTRY ADDRESS
    .align MULTIBOOT_TAG_ALIGN
    .short MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS_EFI64
    .short 0
    .long 12
    .long _start

    # FRAMEBUFFER
    .align MULTIBOOT_TAG_ALIGN
    .short MULTIBOOT_HEADER_TAG_FRAMEBUFFER
    .short 0
    .long 20
    .long 1200
    .long 768
    .long 32

    # END TAG
    .align MULTIBOOT_TAG_ALIGN
    .short MULTIBOOT_HEADER_TAG_END
    .short 0
    .long 8
multiboot_header_end:

.section .text
.global _start
.type _start, @function
.align 16
_start:
    # SETUP STACK
    mov $stack_top, %rsp
    xor %rbp, %rbp

    # SETUP ARGUMENTS
    mov %eax, %edi
    mov %rbx, %rsi

    # JUMP TO KERNEL
    call kernel_main

.loop: # HALT SYSTEM
    cli
    hlt
    jmp .loop

.section .bss
.align 16
.skip STACK_SIZE
stack_top:
