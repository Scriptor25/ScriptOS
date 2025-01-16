.section .text
.global jump_user_main
.type jump_user_main, @function
jump_user_main:
    /* setup stack frame */
    push %ebp
    mov %esp, %ebp

    /* flush tss */
    mov $0x28, %ax
    ltr %ax

    /* reload data segment */
    mov $0x23, %ax /* <data segment> | 3 */
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    /* jump to user code segment */
    mov 0x8(%ebp), %eax
    push $0x23 /* <data segment> | 3 */
    push %eax
    pushf
    push $0x1b /* <code segment> | 3 */
    push $user_main
    iret

.section .user_text
.global syscall
.type syscall, @function
syscall:
    push %ebp
    mov %esp, %ebp

    add $0x8, %esp
    int $0x80
    sub $0x8, %esp

    mov %ebp, %esp
    pop %ebp
    ret
