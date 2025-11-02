#pragma once

#include <scriptos/common.h>
#include <scriptos/types.h>

#define GENERATE_INTERRUPT(NAME)                                            \
    extern "C" void __##NAME##_handler(interrupt::StackFrame* stack_frame); \
    extern "C" void __##NAME##_proxy(void);

#define GENERATE_INTERRUPT_ERROR(NAME)                                           \
    extern "C" void __##NAME##_handler(interrupt::StackFrameError* stack_frame); \
    extern "C" void __##NAME##_proxy(void);

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

    NORETURN void Panic(
        StackFrame* stack_frame,
        bool serious,
        cstr format,
        ...);

    NORETURN void Panic(
        StackFrameError* stack_frame,
        bool serious,
        cstr format,
        ...);
}

GENERATE_INTERRUPT(de)
GENERATE_INTERRUPT(db)
GENERATE_INTERRUPT(ni)
GENERATE_INTERRUPT(bp)
GENERATE_INTERRUPT(of)
GENERATE_INTERRUPT(br)
GENERATE_INTERRUPT(ud)
GENERATE_INTERRUPT(nm)
GENERATE_INTERRUPT_ERROR(df)
GENERATE_INTERRUPT_ERROR(ts)
GENERATE_INTERRUPT_ERROR(np)
GENERATE_INTERRUPT_ERROR(ss)
GENERATE_INTERRUPT_ERROR(gp)
GENERATE_INTERRUPT_ERROR(pf)
GENERATE_INTERRUPT(mf)
GENERATE_INTERRUPT_ERROR(ac)
GENERATE_INTERRUPT(mc)
GENERATE_INTERRUPT(xm)
GENERATE_INTERRUPT(ve)
GENERATE_INTERRUPT_ERROR(cp)

GENERATE_INTERRUPT(irq0)
GENERATE_INTERRUPT(irq1)
GENERATE_INTERRUPT(irq2)
GENERATE_INTERRUPT(irq3)
GENERATE_INTERRUPT(irq4)
GENERATE_INTERRUPT(irq5)
GENERATE_INTERRUPT(irq6)
GENERATE_INTERRUPT(irq7)
GENERATE_INTERRUPT(irq8)
GENERATE_INTERRUPT(irq9)
GENERATE_INTERRUPT(irq10)
GENERATE_INTERRUPT(irq11)
GENERATE_INTERRUPT(irq12)
GENERATE_INTERRUPT(irq13)
GENERATE_INTERRUPT(irq14)
GENERATE_INTERRUPT(irq15)

GENERATE_INTERRUPT(ke)
