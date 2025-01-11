.global LoadGDT
LoadGDT:
    push %ebp
    mov %esp, %ebp

    mov 8(%ebp), %eax
    lgdt (%eax)

    mov 12(%ebp), %eax
    push %eax
    push $.reload_cs
    ljmp *(%esp)

.reload_cs:
    mov 16(%ebp), %eax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    mov %ebp, %esp
    pop %ebp
    ret
