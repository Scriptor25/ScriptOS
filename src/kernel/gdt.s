.global LoadGDT
LoadGDT:
    push %ebp
    mov %esp, %ebp

    mov 0x08(%ebp), %eax
    lgdt (%eax)

    mov 0x0c(%ebp), %eax
    push %eax
    push $.reload_cs
    ljmp *(%esp)

.reload_cs:
    mov 0x10(%ebp), %eax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    mov %ebp, %esp
    pop %ebp
    ret
