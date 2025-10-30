.macro proxy name

    .global \name\()_Proxy
    .type \name\()_Proxy, @function
    .align 16
\name\()_Proxy:
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
    call \name\()_Handler

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

proxy DE
proxy DB
proxy NI
proxy BP
proxy OF
proxy BR
proxy UD
proxy NM
proxy DF
proxy TS
proxy NP
proxy SS
proxy GP
proxy PF
proxy MF
proxy AC
proxy MC
proxy XM
proxy VE
proxy CP
proxy IRQ0
proxy IRQ1
proxy IRQ2
proxy IRQ3
proxy IRQ4
proxy IRQ5
proxy IRQ6
proxy IRQ7
proxy IRQ8
proxy IRQ9
proxy IRQA
proxy IRQB
proxy IRQC
proxy IRQD
proxy IRQE
proxy IRQF
proxy KE
