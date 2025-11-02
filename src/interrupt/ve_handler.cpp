#include <scriptos/interrupt.h>

extern "C" void __ve_handler(interrupt::StackFrame* stack_frame)
{
    interrupt::Panic(
        stack_frame,
        false,
        "Virtualization Exception (%02X:%016X, %02X:%016X)",
        stack_frame->cs,
        stack_frame->rip,
        stack_frame->ss,
        stack_frame->rsp);
}
