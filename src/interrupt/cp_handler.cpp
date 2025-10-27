#include <scriptos/interrupt.h>

extern "C" void CP_Handler(interrupt::StackFrameError* stack_frame)
{
    interrupt::Panic(
        false,
        "Control Protection Exception %016X (%02X:%016X, %02X:%016X)",
        stack_frame->error_code,
        stack_frame->cs,
        stack_frame->rip,
        stack_frame->ss,
        stack_frame->rsp);
}
