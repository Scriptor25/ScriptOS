#include <scriptos/interrupt.h>
#include <scriptos/pic.h>
#include <scriptos/task/schedule.h>
#include <scriptos/task/task.h>
#include <scriptos/types.h>

INTERRUPT void interrupt::IRQ0_Handler(StackFrame* stack_frame)
{
    (void) stack_frame;

    if (task::CurrentTask)
    {
        auto regs = &task::CurrentTask->Regs;

        regs->rflags = stack_frame->Flags;
        regs->cs = stack_frame->CS;
        regs->rip = stack_frame->IP;
        regs->ss = stack_frame->SS;
        regs->rsp = stack_frame->SP;

        __save_context(regs);
    }

    auto next = task::Schedule();

    if (next && next != task::CurrentTask)
    {
        task::CurrentTask = next;
        task::CurrentTask->State = task::TaskState_Running;
        __restore_context(&task::CurrentTask->Regs);
    }

    pic::SendEOI(0x0);
}
