#include <scriptos/asm.h>
#include <scriptos/interrupt.h>
#include <scriptos/pic.h>
#include <scriptos/processor.h>
#include <scriptos/task/schedule.h>
#include <scriptos/task/task.h>
#include <scriptos/types.h>

extern "C" void IRQ0_Handler(interrupt::StackFrame* stack_frame)
{
    if (processor::NoProcessorState())
    {
        pic::SendEOI(0x0);
        return;
    }

    auto active = processor::GetProcessorActiveTask();
    if (active)
    {
        active->Frame = *stack_frame;
    }

    auto next = task::NextTask(active);
    if (next)
    {
        next->State = task::TaskState_Running;
        if (next != active)
        {
            *stack_frame = next->Frame;
        }
    }

    if (next != active)
    {
        processor::SetProcessorActiveTask(next);
    }

    pic::SendEOI(0x0);
}
