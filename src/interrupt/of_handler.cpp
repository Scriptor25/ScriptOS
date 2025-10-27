#include <scriptos/interrupt.h>

extern "C" void OF_Handler(interrupt::StackFrame* stack_frame)
{
    interrupt::Panic(
        false,
        "Overflow (%02X:%016X, %02X:%016X)",
        stack_frame->cs,
        stack_frame->rip,
        stack_frame->ss,
        stack_frame->rsp);
}
