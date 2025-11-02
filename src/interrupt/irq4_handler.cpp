#include <scriptos/interrupt.h>
#include <scriptos/pic.h>

extern "C" void __irq4_handler(interrupt::StackFrame* stack_frame)
{
    (void) stack_frame;

    pic::SendEOI(0x4);
}
