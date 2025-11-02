#pragma once

#include <scriptos/types.h>

#define GENERATE_INTERRUPT(NAME)                                        \
    extern "C" void NAME##_Handler(interrupt::StackFrame* stack_frame); \
    extern "C" void NAME##_Proxy(void);

#define GENERATE_INTERRUPT_ERROR(NAME)                                       \
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
        inline operator StackFrame() const
        {
            return {
                .r15 = r15,
                .r14 = r14,
                .r13 = r13,
                .r12 = r12,
                .r11 = r11,
                .r10 = r10,
                .r9 = r9,
                .r8 = r8,
                .rsi = rsi,
                .rdi = rdi,
                .rbp = rbp,
                .rdx = rdx,
                .rcx = rcx,
                .rbx = rbx,
                .rax = rax,
                .rip = rip,
                .cs = cs,
                .rflags = rflags,
                .rsp = rsp,
                .ss = ss,
            };
        }

        u64 r15, r14, r13, r12, r11, r10, r9, r8;
        u64 rsi, rdi, rbp, rdx, rcx, rbx, rax;
        u64 error_code;
        u64 rip, cs, rflags, rsp, ss;
    };

    __attribute__((noreturn)) void Panic(
        StackFrame* stack_frame,
        bool serious,
        cstr format,
        ...);

    __attribute__((noreturn)) void Panic(
        StackFrameError* stack_frame,
        bool serious,
        cstr format,
        ...);
}

GENERATE_INTERRUPT(DE)
GENERATE_INTERRUPT(DB)
GENERATE_INTERRUPT(NI)
GENERATE_INTERRUPT(BP)
GENERATE_INTERRUPT(OF)
GENERATE_INTERRUPT(BR)
GENERATE_INTERRUPT(UD)
GENERATE_INTERRUPT(NM)
GENERATE_INTERRUPT_ERROR(DF)
GENERATE_INTERRUPT_ERROR(TS)
GENERATE_INTERRUPT_ERROR(NP)
GENERATE_INTERRUPT_ERROR(SS)
GENERATE_INTERRUPT_ERROR(GP)
GENERATE_INTERRUPT_ERROR(PF)
GENERATE_INTERRUPT(MF)
GENERATE_INTERRUPT_ERROR(AC)
GENERATE_INTERRUPT(MC)
GENERATE_INTERRUPT(XM)
GENERATE_INTERRUPT(VE)
GENERATE_INTERRUPT_ERROR(CP)

GENERATE_INTERRUPT(IRQ0)
GENERATE_INTERRUPT(IRQ1)
GENERATE_INTERRUPT(IRQ2)
GENERATE_INTERRUPT(IRQ3)
GENERATE_INTERRUPT(IRQ4)
GENERATE_INTERRUPT(IRQ5)
GENERATE_INTERRUPT(IRQ6)
GENERATE_INTERRUPT(IRQ7)
GENERATE_INTERRUPT(IRQ8)
GENERATE_INTERRUPT(IRQ9)
GENERATE_INTERRUPT(IRQA)
GENERATE_INTERRUPT(IRQB)
GENERATE_INTERRUPT(IRQC)
GENERATE_INTERRUPT(IRQD)
GENERATE_INTERRUPT(IRQE)
GENERATE_INTERRUPT(IRQF)

GENERATE_INTERRUPT(KE)
