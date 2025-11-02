#include <scriptos/asm.h>
#include <scriptos/common.h>
#include <scriptos/interrupt.h>
#include <scriptos/print.h>
#include <scriptos/serial.h>
#include <scriptos/types.h>

NORETURN static void __panic(
    const interrupt::StackFrame& stack_frame,
    bool serious,
    cstr format,
    va_list& ap)
{
    out_stream stream = serious ? serial::WriteDefault : kstdout;

    vfkprintf(stream, format, ap);
    fkputs(stream, "\r\n");

    fkprintf(
        stream,
        "r15=%016x r14=%016x r13=%016x r12=%016x\r\n"
        "r11=%016x r10=%016x r9=%016x r8=%016x\r\n"
        "rsi=%016x rdi=%016x rbp=%016x\r\n"
        "rdx=%016x rcx=%016x rbx=%016x rax=%016x\r\n"
        "rip=%016x cs=%04x\r\n"
        "rflags=%016x\r\n"
        "rsp=%016x ss=%04x\r\n",
        stack_frame.r15,
        stack_frame.r14,
        stack_frame.r13,
        stack_frame.r12,
        stack_frame.r11,
        stack_frame.r10,
        stack_frame.r9,
        stack_frame.r8,
        stack_frame.rsi,
        stack_frame.rdi,
        stack_frame.rbp,
        stack_frame.rdx,
        stack_frame.rcx,
        stack_frame.rbx,
        stack_frame.rax,
        stack_frame.rip,
        stack_frame.cs,
        stack_frame.rflags,
        stack_frame.rsp,
        stack_frame.ss);

    if (!serious)
    {
        kflush();
    }

    for (;;)
    {
        hlt();
    }
}

NORETURN void interrupt::Panic(
    interrupt::StackFrame* stack_frame,
    bool serious,
    cstr format,
    ...)
{
    va_list ap;
    va_start(ap, format);
    __panic(*stack_frame, serious, format, ap);
    va_end(ap);
}

NORETURN void interrupt::Panic(
    interrupt::StackFrameError* stack_frame,
    bool serious,
    cstr format,
    ...)
{
    va_list ap;
    va_start(ap, format);
    __panic(*stack_frame, serious, format, ap);
    va_end(ap);
}
