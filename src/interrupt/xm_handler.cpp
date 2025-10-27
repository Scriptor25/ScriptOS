#include <scriptos/interrupt.h>

extern "C" void XM_Handler(interrupt::StackFrame* stack_frame)
{
    interrupt::Panic(
        false,
        "SIMD Floating-Point Exception (%02X:%016X, %02X:%016X)",
        stack_frame->cs,
        stack_frame->rip,
        stack_frame->ss,
        stack_frame->rsp);
}
