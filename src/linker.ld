OUTPUT_FORMAT(elf64-x86-64)

ENTRY(multiboot_entry)

SECTIONS
{
    . = 1M;

    .multiboot : {
        __multiboot = .;
        
        KEEP(*(.multiboot))

        __multiboot_end = .;
    }

    .text : {
        __text = .;
        
        *(.text*)
        
        __text_end = .;
    }

    .rodata : {
        __rodata = .;

        *(.rodata*)

        __rodata_end = .;
    }

    .data : {
        __data = .;

        *(.data*)

        __data_end = .;
    }

    .bss : {
        __bss = .;

        *(.bss*)
        *(COMMON)

        __bss_end = .;
    }

    /DISCARD/ : {
        *(.note*)
        *(.eh_frame*)
        *(.comment)
    }
}
