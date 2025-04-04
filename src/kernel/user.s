.section .text
.global jump_user
.type jump_user, @function
jump_user:
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

    /* load arguments into scratch registers */
    mov 0x8(%ebp), %eax /* user stack address */
    mov 0xC(%ebp), %ecx /* user main address */

    /* jump to user code segment */
    push $0x23 /* <data segment> | 3 */
    push %eax
    pushf
    push $0x1b /* <code segment> | 3 */
    push %ecx
    iret
