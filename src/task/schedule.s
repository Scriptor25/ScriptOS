.global __save_context
.type __save_context, @function
__save_context:
    /* save general-purpose registers */
    movq %r15, 0x00(%rdi)
    movq %r14, 0x08(%rdi)
    movq %r13, 0x10(%rdi)
    movq %r12, 0x18(%rdi)
    movq %r11, 0x20(%rdi)
    movq %r10, 0x28(%rdi)
    movq %r9,  0x30(%rdi)
    movq %r8,  0x38(%rdi)
    movq %rsi, 0x40(%rdi)
    movq %rdi, 0x48(%rdi)
    movq %rbp, 0x50(%rdi)
    movq %rdx, 0x58(%rdi)
    movq %rcx, 0x60(%rdi)
    movq %rbx, 0x68(%rdi)
    movq %rax, 0x70(%rdi)

    ret

.global __restore_context
.type __restore_context, @function
__restore_context:
    /* restore segment registers */
    movq 0x98(%rdi), %rax
    mov  %ax, %ds
    mov  %ax, %es
    mov  %ax, %fs
    mov  %ax, %gs

    /* restore rsp */
    movq 0x90(%rdi), %rsp

    /* build stack frame */
    pushq 0x98(%rdi) /* restore ss */
    pushq 0x90(%rdi) /* restore rsp */
    pushq 0x88(%rdi) /* restore rflags */
    pushq 0x80(%rdi) /* restore cs */
    pushq 0x78(%rdi) /* restore rip */

    /* restore general-purpose registers */
    movq 0x00(%rdi), %r15
    movq 0x08(%rdi), %r14
    movq 0x10(%rdi), %r13
    movq 0x18(%rdi), %r12
    movq 0x20(%rdi), %r11
    movq 0x28(%rdi), %r10
    movq 0x30(%rdi), %r9
    movq 0x38(%rdi), %r8
    movq 0x40(%rdi), %rsi
    movq 0x50(%rdi), %rbp
    movq 0x58(%rdi), %rdx
    movq 0x60(%rdi), %rcx
    movq 0x68(%rdi), %rbx
    movq 0x70(%rdi), %rax

    /* restore rdi */
    movq 0x48(%rdi), %rdi

    /* send eoi */
    pushq %rax
    pushq %rdx
    movb $0x20, %al
    movw $0x0020, %dx
    outb %al, %dx
    popq %rdx
    popq %rax

    iretq
