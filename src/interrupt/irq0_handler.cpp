#include <scriptos/asm.h>
#include <scriptos/interrupt.h>
#include <scriptos/pic.h>
#include <scriptos/processor.h>
#include <scriptos/task.h>
#include <scriptos/types.h>

extern "C" void __irq0_handler(interrupt::StackFrame* stack_frame)
{
    if (kernel::NoProcessorState())
    {
        kernel::SendPicEoi(0x0);
        return;
    }

    auto active = kernel::GetProcessorActiveTask();
    if (active)
        active->Frame = *stack_frame;

    auto next = kernel::NextTask(active);
    if (next)
    {
        next->State = kernel::TaskState_Running;
        if (next != active)
            *stack_frame = next->Frame;
    }

    if (next != active)
        kernel::SetProcessorActiveTask(next);

    kernel::SendPicEoi(0x0);
}
