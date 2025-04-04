.section .text
.global syscall
.type syscall, @function
syscall:
    /* setup stack frame */
    push %ebp
    mov %esp, %ebp

    /* interrupt for syscall */
    add $0x8, %esp
    int $0x80
    sub $0x8, %esp

    /* clean up stack frame */
    mov %ebp, %esp
    pop %ebp
    ret
