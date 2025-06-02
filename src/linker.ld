OUTPUT_FORMAT(elf64-x86-64)

ENTRY(kmain)

SECTIONS
{
    . = 0xffffffff80000000;

    .limine_requests :
    {
        KEEP(*(.limine_requests_start))
        KEEP(*(.limine_requests))
        KEEP(*(.limine_requests_end))
    } : limine_requests

    .text BLOCK(4K) : ALIGN(4K)
    {
        __text = .;
        *(.text*)
        __text_end = .;
    } : text

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        __rodata = .;
        *(.rodata*)
        __rodata_end = .;
    } : rodata

    .data BLOCK(4K) : ALIGN(4K)
    {
        __data = .;
        *(.data*)
        __data_end = .;
    } : data

    __load_end = .;

    .bss BLOCK(4K) : ALIGN(4K)
    {
        __bss = .;
        *(.bss*)
        *(COMMON)
        __bss_end = .;
    } : data

    /DISCARD/ :
    {
        *(.eh_frame*)
        *(.note*)
    }
}

PHDRS
{
    limine_requests PT_LOAD            ;
    text            PT_LOAD FLAGS(101b); /* rx */
    rodata          PT_LOAD FLAGS(100b); /* ro */
    data            PT_LOAD FLAGS(110b); /* rw */
}
