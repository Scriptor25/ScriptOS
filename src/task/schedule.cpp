#include <scriptos/memory.h>
#include <scriptos/paging.h>
#include <scriptos/task/schedule.h>
#include <scriptos/task/task.h>
#include <scriptos/types.h>

static u64 next_pid = 0;
static task::Task* task_queue_begin = nullptr;

task::Task* task::CurrentTask = nullptr;

__attribute__((noreturn)) static void task_exit()
{
    asm volatile("cli");

    task::CurrentTask->State = task::TaskState_Zombie;

    asm volatile("sti");

    for (;;)
    {
        asm volatile("hlt");
    }
}

#define STACK_SIZE 0x2000

task::Task* task::Create(
    cstr name,
    u64 priority,
    void (*entry)(void*),
    void* arg)
{
    auto task = memory::Allocate<Task>();
    {
        auto kernel_stack = memory::Allocate(STACK_SIZE);
        memory::Fill(kernel_stack, 0, STACK_SIZE);

        *task = {
            .PID = next_pid++,
            .Name = {},
            .Regs = {},
            .CR3 = 0,
            .KernelStack = kernel_stack,
            .FxState = nullptr,
            .State = TaskState_Runnable,
            .Priority = priority,
            .TimesliceMillis = TASK_TIMESLICE_MILLIS,
            .PrevTask = nullptr,
            .NextTask = nullptr,
        };

        auto len = memory::StringLength(name);
        memory::Copy(task->Name, name, len < TASK_NAME_LEN ? len : TASK_NAME_LEN);
    }

    auto stack_top = reinterpret_cast<u8*>(task->KernelStack) + STACK_SIZE;
    stack_top = reinterpret_cast<u8*>(reinterpret_cast<uptr>(stack_top) & ~0xF);

    stack_top -= 8;
    *reinterpret_cast<u64*>(stack_top) = reinterpret_cast<u64>(task_exit);

    task->Regs.rip = reinterpret_cast<u64>(entry);
    task->Regs.rsp = reinterpret_cast<u64>(stack_top);
    task->Regs.rflags = 0x202;
    task->Regs.cs = 0x08;
    task->Regs.ss = 0x10;

    task->Regs.rdi = reinterpret_cast<u64>(arg);

    return task;
}

void task::Enqueue(Task* task)
{
    task->PrevTask = nullptr;
    task->NextTask = nullptr;

    if (!task_queue_begin)
    {
        task_queue_begin = task;
    }
    else
    {
        Task* it;
        for (it = task_queue_begin; it->NextTask; it = it->NextTask)
            ;

        it->NextTask = task;
        task->PrevTask = it;
    }
}

task::Task* task::Schedule()
{
    Task* task;

    Reaper();

    if (!task_queue_begin)
    {
        return nullptr;
    }

    if (!CurrentTask)
    {
        return task_queue_begin;
    }

    if (!CurrentTask->NextTask)
    {
        for (task = task_queue_begin; task; task = task->NextTask)
        {
            task->State = TaskState_Runnable;
        }
        return task_queue_begin;
    }

    for (task = CurrentTask; task && task->State != TaskState_Runnable; task = task->NextTask)
        ;

    return task;
}

void task::Reaper()
{
    for (auto task = task_queue_begin; task && task->NextTask;)
    {
        if (task->State == TaskState_Zombie)
        {
            if (task->PrevTask)
            {
                task->PrevTask->NextTask = task->NextTask;
            }
            else if (task == task_queue_begin)
            {
                task_queue_begin = task->NextTask;
            }

            if (task->NextTask)
            {
                task->NextTask->PrevTask = task->PrevTask;
            }

            auto next = task->NextTask;

            memory::Free(task->KernelStack);
            memory::Free(task);

            task = next;
            continue;
        }
        task = task->NextTask;
    }
}
