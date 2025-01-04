#define ASM_FILE 1
#include <multiboot2.h>

.extern _data_end
.extern _bss_end

.set MAGIC,         MULTIBOOT2_HEADER_MAGIC
.set ARCHITECTURE,  MULTIBOOT_ARCHITECTURE_I386
.set HEADER_LENGTH, multiboot_header_end - multiboot_header
.set CHECKSUM,      -(MAGIC + ARCHITECTURE + HEADER_LENGTH)

.section .multiboot
.align 8
multiboot_header:
.long MAGIC
.long ARCHITECTURE
.long HEADER_LENGTH
.long CHECKSUM

address_tag_start:
.short MULTIBOOT_HEADER_TAG_ADDRESS
.short MULTIBOOT_HEADER_TAG_OPTIONAL
.long address_tag_end - address_tag_start
.long multiboot_header
.long _start
.long _data_end
.long _bss_end
address_tag_end:

entry_address_tag_start:
.short MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS
.short MULTIBOOT_HEADER_TAG_OPTIONAL
.long entry_address_tag_end - entry_address_tag_start
.long _start
.long 0 /* alignment padding */
entry_address_tag_end:

framebuffer_tag_start:
.short MULTIBOOT_HEADER_TAG_FRAMEBUFFER
.short MULTIBOOT_HEADER_TAG_OPTIONAL
.long framebuffer_tag_end - framebuffer_tag_start
.long 1024
.long 768
.long 32
.long 0 /* alignment padding */
framebuffer_tag_end:

.short MULTIBOOT_HEADER_TAG_END
.short 0
.long 8
multiboot_header_end:

.section .bss
.align 16
stack_bottom:
.skip 0x4000
stack_top:

.section .text
.global _start
_start:
        /* setup stack */
        mov $stack_top, %esp

        /* reset eflags */
        pushl $0
        popf

        /* push multiboot info pointer */
        pushl %ebx
        /* push magic value */
        pushl %eax

        /* enter kernel main */
        call main

        /* halt */
.loop:  cli
        hlt
        jmp .loop

.size _start, . - _start
