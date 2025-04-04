ENTRY(user_main)

SECTIONS
{
    __user_start = .;

    .text BLOCK(4K) : ALIGN(4K)
    {
        __user_text_start = .;
        *(.text)
        __user_text_end = .;
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        __user_rodata_start = .;
        *(.rodata)
        __user_rodata_end = .;
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        __user_data_start = .;
        *(.data)
        __user_data_end = .;
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        __user_bss_start = .;
        *(COMMON)
        *(.bss)
        __user_bss_end = .;
    }
    
    __user_end = .;
}
