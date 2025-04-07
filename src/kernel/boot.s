#define ASM_FILE 1
#include <scriptos/kernel/multiboot2.hpp>

.set MAGIC,         MULTIBOOT2_HEADER_MAGIC
.set ARCHITECTURE,  MULTIBOOT_ARCHITECTURE_I386
.set HEADER_LENGTH, multiboot_header_end - multiboot_header
.set CHECKSUM,      -(MAGIC + ARCHITECTURE + HEADER_LENGTH)
.set STACK_SIZE,    0x4000

.section .text
.global _start
_start:
        jmp multiboot_entry

        .align 8
multiboot_header:
        .long MAGIC
        .long ARCHITECTURE
        .long HEADER_LENGTH
        .long CHECKSUM

        .align 8
address_tag_start:
        .short MULTIBOOT_HEADER_TAG_ADDRESS
        .short MULTIBOOT_HEADER_TAG_OPTIONAL
        .long address_tag_end - address_tag_start
        .long multiboot_header
        .long _start
        .long __data_end
        .long __bss_end
address_tag_end:

        .align 8
entry_address_tag_start:
        .short MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS
        .short MULTIBOOT_HEADER_TAG_OPTIONAL
        .long entry_address_tag_end - entry_address_tag_start
        .long _start
entry_address_tag_end:

        .align 8
framebuffer_tag_start:
        .short MULTIBOOT_HEADER_TAG_FRAMEBUFFER
        .short MULTIBOOT_HEADER_TAG_OPTIONAL
        .long framebuffer_tag_end - framebuffer_tag_start
        .long 1200
        .long 768
        .long 32
framebuffer_tag_end:

        .align 8
        .short MULTIBOOT_HEADER_TAG_END
        .short 0
        .long 8
multiboot_header_end:

multiboot_entry:
        /* setup stack */
        mov $stack_top, %esp

        /* reset eflags */
        push $0
        popf

        /* push multiboot info pointer */
        push %ebx
        /* push magic value */
        push %eax

        /* enter kernel main */
        call kernel_main

        /* halt */
.loop:  cli
        hlt
        jmp .loop

.section .bss
stack_bottom:
.skip STACK_SIZE
stack_top:
