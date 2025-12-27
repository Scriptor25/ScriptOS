.macro proxy name

    .global __\name\()_proxy
    .type __\name\()_proxy, @function
    .align 16
__\name\()_proxy:
    cli

    /* save context */
    pushq %rax
    pushq %rbx
    pushq %rcx
    pushq %rdx
    pushq %rbp
    pushq %rdi
    pushq %rsi
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15

    /* save pointer to context */
    movq %rsp, %rdi

    /* call handler */
    call __\name\()_handler

    /* restore context */
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rsi
    popq %rdi
    popq %rbp
    popq %rdx
    popq %rcx
    popq %rbx
    popq %rax

    sti

    /* return */
    iretq

.endm

proxy de
proxy db
proxy ni
proxy bp
proxy of
proxy br
proxy ud
proxy nm
proxy df
proxy ts
proxy np
proxy ss
proxy gp
proxy pf
proxy mf
proxy ac
proxy mc
proxy xm
proxy ve
proxy cp
proxy irq0
proxy irq1
proxy irq2
proxy irq3
proxy irq4
proxy irq5
proxy irq6
proxy irq7
proxy irq8
proxy irq9
proxy irq10
proxy irq11
proxy irq12
proxy irq13
proxy irq14
proxy irq15
proxy ke
proxy sys
