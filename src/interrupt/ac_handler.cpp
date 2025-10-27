#include <scriptos/interrupt.h>

extern "C" void AC_Handler(interrupt::StackFrameError* stack_frame)
{
    interrupt::Panic(
        false,
        "Alignment Check %016X (%02X:%016X, %02X:%016X)",
        stack_frame->error_code,
        stack_frame->cs,
        stack_frame->rip,
        stack_frame->ss,
        stack_frame->rsp);
}
