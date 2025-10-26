#include <scriptos/interrupt.h>
#include <scriptos/pic.h>

INTERRUPT void interrupt::IRQD_Handler(StackFrame* stack_frame)
{
    (void) stack_frame;

    pic::SendEOI(0xD);
}
