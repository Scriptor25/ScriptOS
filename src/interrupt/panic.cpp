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
    out_stream stream = serious ? kernel::WriteSerialDefault : kstdout;

    vfkprintf(stream, format, ap);
    fkputs(stream, "\r\n");

    fkprintf(
        stream,
        "r15=%016llx r14=%016llx r13=%016llx r12=%016llx\r\n"
        "r11=%016llx r10=%016llx r9=%016llx r8=%016llx\r\n"
        "rsi=%016llx rdi=%016llx rbp=%016llx\r\n"
        "rdx=%016llx rcx=%016llx rbx=%016llx rax=%016llx\r\n"
        "rip=%016llx cs=%04x\r\n"
        "rflags=%016llx\r\n"
        "rsp=%016llx ss=%04x\r\n",
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
