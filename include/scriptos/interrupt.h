#pragma once

#include <scriptos/types.h>

#define INTERRUPT(NAME)                                                 \
    extern "C" void NAME##_Handler(interrupt::StackFrame* stack_frame); \
    extern "C" void NAME##_Proxy(void);

#define INTERRUPT_ERROR(NAME)                                                \
    extern "C" void NAME##_Handler(interrupt::StackFrameError* stack_frame); \
    extern "C" void NAME##_Proxy(void);

namespace interrupt
{
    struct StackFrame
    {
        u64 r15, r14, r13, r12, r11, r10, r9, r8;
        u64 rsi, rdi, rbp, rdx, rcx, rbx, rax;
        u64 rip, cs, rflags, rsp, ss;
    };

    struct StackFrameError
    {
        u64 r15, r14, r13, r12, r11, r10, r9, r8;
        u64 rsi, rdi, rbp, rdx, rcx, rbx, rax;
        u64 error_code;
        u64 rip, cs, rflags, rsp, ss;
    };

    void Panic(
        bool serious,
        cstr format,
        ...);
}

INTERRUPT(DE)
INTERRUPT(DB)
INTERRUPT(NI)
INTERRUPT(BP)
INTERRUPT(OF)
INTERRUPT(BR)
INTERRUPT(UD)
INTERRUPT(NM)
INTERRUPT_ERROR(DF)
INTERRUPT_ERROR(TS)
INTERRUPT_ERROR(NP)
INTERRUPT_ERROR(SS)
INTERRUPT_ERROR(GP)
INTERRUPT_ERROR(PF)
INTERRUPT(MF)
INTERRUPT_ERROR(AC)
INTERRUPT(MC)
INTERRUPT(XM)
INTERRUPT(VE)
INTERRUPT_ERROR(CP)

INTERRUPT(IRQ0)
INTERRUPT(IRQ1)
INTERRUPT(IRQ2)
INTERRUPT(IRQ3)
INTERRUPT(IRQ4)
INTERRUPT(IRQ5)
INTERRUPT(IRQ6)
INTERRUPT(IRQ7)
INTERRUPT(IRQ8)
INTERRUPT(IRQ9)
INTERRUPT(IRQA)
INTERRUPT(IRQB)
INTERRUPT(IRQC)
INTERRUPT(IRQD)
INTERRUPT(IRQE)
INTERRUPT(IRQF)

INTERRUPT(KE)
