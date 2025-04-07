.global LoadLDT
LoadLDT:
    push %ebp
    mov %esp, %ebp

    mov 0x08(%ebp), %eax
    lldt (%eax)

    mov %ebp, %esp
    pop %ebp
    ret
