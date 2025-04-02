ENTRY(_start)

SECTIONS
{
    . = 2M;

    __kernel_start = .;

    .text BLOCK(4K) : ALIGN(4K)
    {
        __text_start = .;
        *(.text)
        __text_end = .;
    }

    .user_text BLOCK(4K) : ALIGN(4K)
    {
        __user_text_start = .;
        *(.user_text)
        __user_text_end = .;
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        __rodata_start = .;
        *(.rodata)
        __rodata_end = .;
    }

    .user_rodata BLOCK(4K) : ALIGN(4K)
    {
        __user_rodata_start = .;
        *(.user_rodata)
        __user_rodata_end = .;
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        __data_start = .;
        *(.data)
        __data_end = .;
    }

    .user_data BLOCK(4K) : ALIGN(4K)
    {
        __user_data_start = .;
        *(.user_data)
        __user_data_end = .;
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        __bss_start = .;
        *(COMMON)
        *(.bss)
        __bss_end = .;
    }

    .user_bss BLOCK(4K) : ALIGN(4K)
    {
        __user_bss_start = .;
        *(COMMON)
        *(.user_bss)
        __user_bss_end = .;
    }

    __kernel_end = .;
}
