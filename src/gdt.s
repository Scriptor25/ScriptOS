.global __load_gdt
.type __load_gdt, @function
__load_gdt:
    lgdt (%rdi)
    
    mov %dx, %ds
    mov %dx, %es
    mov %dx, %fs
    mov %dx, %gs
    mov %dx, %ss

    pop %rax
    push %rsi
    push %rax
    
    retfq
