#include <scriptos/interrupt.h>

extern "C" void DB_Handler(interrupt::StackFrame* stack_frame)
{
    interrupt::Panic(
        false,
        "Debug Exception (%02X:%016X, %02X:%016X)",
        stack_frame->cs,
        stack_frame->rip,
        stack_frame->ss,
        stack_frame->rsp);
}
