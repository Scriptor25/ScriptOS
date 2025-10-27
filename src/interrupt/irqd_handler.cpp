#include <scriptos/interrupt.h>
#include <scriptos/pic.h>

extern "C" void IRQD_Handler(interrupt::StackFrame* stack_frame)
{
    (void) stack_frame;

    pic::SendEOI(0xD);
}
