#include <scriptos/interrupt.h>
#include <scriptos/pic.h>

extern "C" void __irq9_handler(interrupt::StackFrame* stack_frame)
{
    (void) stack_frame;

    kernel::SendPicEoi(0x9);
}
