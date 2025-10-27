#include <scriptos/interrupt.h>
#include <scriptos/pic.h>
#include <scriptos/task/schedule.h>
#include <scriptos/task/task.h>
#include <scriptos/types.h>

extern "C" void IRQ0_Handler(interrupt::StackFrame* stack_frame)
{
    auto active = task::ActiveTask;
    if (active)
    {
        active->Frame = *stack_frame;
    }

    auto next = task::NextTask();
    task::ActiveTask = next;

    if (next && next != active)
    {
        next->State = task::TaskState_Running;
        *stack_frame = next->Frame;
    }

    pic::SendEOI(0x0);
}
