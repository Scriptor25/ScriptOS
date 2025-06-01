OUTPUT_FORMAT(elf64-x86-64)

ENTRY(_start)

SECTIONS
{
    . = 2M;

    .text BLOCK(4K) : ALIGN(4K)
    {
        __text = .;
        *(.text)
        __text_end = .;
    } : text

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        __rodata = .;
        *(.rodata)
        __rodata_end = .;
    } : rodata

    .data BLOCK(4K) : ALIGN(4K)
    {
        __data = .;
        *(.data)
        __data_end = .;
    } : data

    .bss BLOCK(4K) : ALIGN(4K)
    {
        __bss = .;
        *(COMMON)
        *(.bss)
        __bss_end = .;
    } : data
}

PHDRS
{
    text   PT_LOAD FLAGS(5);
    rodata PT_LOAD FLAGS(4);
    data   PT_LOAD FLAGS(6);
}
