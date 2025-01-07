ENTRY(_start)

SECTIONS
{
    . = 2M;

    _kernel_start = .;

    .text BLOCK(8K) : ALIGN(8K)
    {
        _text_start = .;
        *(.text)
        _text_end = .;
    }

    .rodata BLOCK(8K) : ALIGN(8K)
    {
        _rodata_start = .;
        *(.rodata)
        _rodata_end = .;
    }

    .data BLOCK(8K) : ALIGN(8K)
    {
        _data_start = .;
        *(.data)
        _data_end = .;
    }

    .bss BLOCK(8K) : ALIGN(8K)
    {
        _bss_start = .;
        *(COMMON)
        *(.bss)
        _bss_end = .;
    }

    _kernel_end = .;
}
