.global __task_trampoline
__task_trampoline:
    popq %rdx /* entry */
    popq %rsi /* task */
    popq %rdi /* arg */

    /* call entry with %rdi=arg */
    call *%rdx
    
    /* call __task_exit with %rdi=task */
    mov %rsi, %rdi
    jmp __task_exit
