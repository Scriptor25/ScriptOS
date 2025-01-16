.global jump_user_main
.type jump_user_main, @function
jump_user_main:
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
    mov %esp, %eax
    push $0x23 /* <data segment> | 3 */
    push %eax
    pushf
    push $0x1b /* <code segment> | 3 */
    push $user_main
    iret
