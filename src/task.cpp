#include <scriptos/asm.h>
#include <scriptos/kernel.h>
#include <scriptos/memory.h>
#include <scriptos/paging.h>
#include <scriptos/print.h>
#include <scriptos/processor.h>
#include <scriptos/task.h>
#include <scriptos/types.h>

static u64 task_next_pid             = 0;
static kernel::Task* task_queue_root = nullptr;

static void __task_exit()
{
    cli();

    auto task   = kernel::GetProcessorActiveTask();
    task->State = kernel::TaskState_Zombie;

    sti();

    for (;;)
    {
        hlt();
    }
}

kernel::Task* kernel::CreateTask(
    cstr name,
    u64 priority,
    void (*entry)(void*),
    void* arg)
{
    auto task = kernel::Allocate<Task>();

    auto pid = task_next_pid++;

    auto stack = kernel::Instance.Allocator->AllocatePhysicalPage();
    kernel::MapPage(stack, stack, true);

    kernel::Fill(stack, 0, PAGE_SIZE);

    *task = {
        .PID       = pid,
        .Name      = name,
        .Frame     = {},
        .Stack     = stack,
        .State     = TaskState_Runnable,
        .Priority  = priority,
        .Timeslice = priority ? priority * TASK_TIMESLICE : 1,
        .PrevTask  = nullptr,
        .NextTask  = nullptr,
    };

    auto stack_top = reinterpret_cast<u8*>(stack) + PAGE_SIZE;
    stack_top = reinterpret_cast<u8*>(reinterpret_cast<uptr>(stack_top) & ~0xF);

    *reinterpret_cast<u64*>(stack_top -= 8) = reinterpret_cast<u64>(__task_exit);

    task->Frame.rip    = reinterpret_cast<u64>(entry);
    task->Frame.rsp    = reinterpret_cast<u64>(stack_top);
    task->Frame.rflags = 0x202;
    task->Frame.cs     = 0x08;
    task->Frame.ss     = 0x10;

    task->Frame.rdi = reinterpret_cast<u64>(arg);

    return task;
}

void kernel::EnqueueTask(Task* task)
{
    cli();

    task->PrevTask = nullptr;
    task->NextTask = nullptr;

    if (!task_queue_root)
    {
        task_queue_root = task;
    }
    else
    {
        Task* it;
        for (it = task_queue_root; it->NextTask; it = it->NextTask)
            ;

        it->NextTask   = task;
        task->PrevTask = it;
    }

    sti();
}

kernel::Task* kernel::NextTask(Task* active)
{
    TaskReaper();

    if (!task_queue_root)
    {
        return nullptr;
    }

    if (!active)
    {
        return task_queue_root;
    }

    if (active->Timeslice)
    {
        active->Timeslice--;
        return active;
    }

    if (!active->NextTask)
    {
        for (auto task = task_queue_root; task; task = task->NextTask)
        {
            if (task->State == TaskState_Running)
            {
                task->State = TaskState_Runnable;
                task->Timeslice = task->Priority ? task->Priority * TASK_TIMESLICE : 1;
            }
        }
        return task_queue_root;
    }

    Task* task;
    for (task = active; task && task->State != TaskState_Runnable; task = task->NextTask)
        ;

    return task;
}

void kernel::TaskReaper()
{
    for (auto task = task_queue_root; task;)
    {
        if (task->State == TaskState_Zombie)
        {
            if (task->PrevTask)
            {
                task->PrevTask->NextTask = task->NextTask;
            }
            else
            {
                task_queue_root = task->NextTask;
            }

            if (task->NextTask)
            {
                task->NextTask->PrevTask = task->PrevTask;
            }

            auto next = task->NextTask;

            kernel::Instance.Allocator->FreePage(task->Stack);

            *reinterpret_cast<u64*>(task->Stack) = 0x6969'6969'6969'6969llu;

            kernel::Free(task);

            task = next;
            continue;
        }

        task = task->NextTask;
    }
}
