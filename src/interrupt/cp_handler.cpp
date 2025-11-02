#include <scriptos/interrupt.h>

extern "C" void __cp_handler(interrupt::StackFrameError* stack_frame)
{
    interrupt::Panic(
        stack_frame,
        false,
        "Control Protection Exception %016X (%02X:%016X, %02X:%016X)",
        stack_frame->error_code,
        stack_frame->cs,
        stack_frame->rip,
        stack_frame->ss,
        stack_frame->rsp);
}
