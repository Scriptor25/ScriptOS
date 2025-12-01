#include <scriptos/interrupt.h>
#include <scriptos/pic.h>

extern "C" void __irq15_handler(interrupt::StackFrame* stack_frame)
{
    (void) stack_frame;

    kernel::SendPicEoi(0xF);
}
