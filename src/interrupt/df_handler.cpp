#include <scriptos/interrupt.h>

extern "C" void __df_handler(interrupt::StackFrameError* stack_frame)
{
    interrupt::Panic(
        stack_frame,
        true,
        "Double Fault (%02X:%016X, %02X:%016X)",
        stack_frame->cs,
        stack_frame->rip,
        stack_frame->ss,
        stack_frame->rsp);
}
