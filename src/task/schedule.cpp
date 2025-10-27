#include <scriptos/memory.h>
#include <scriptos/paging.h>
#include <scriptos/task/schedule.h>
#include <scriptos/task/task.h>
#include <scriptos/types.h>

static u64 task_next_pid = 0;
static task::Task* task_queue_root = nullptr;

task::Task* task::ActiveTask = nullptr;

__attribute__((noreturn)) static void task_exit()
{
    asm volatile("cli");

    task::ActiveTask->State = task::TaskState_Zombie;

    asm volatile("sti");

    for (;;)
    {
        asm volatile("hlt");
    }
}

#define STACK_SIZE 0x2000

task::Task* task::CreateTask(
    cstr name,
    u64 priority,
    void (*entry)(void*),
    void* arg)
{
    auto task = memory::Allocate<Task>();

    auto kernel_stack = memory::Allocate(STACK_SIZE);
    memory::Fill(kernel_stack, 0, STACK_SIZE);

    *task = {
        .PID = task_next_pid++,
        .Name = name,
        .Frame = {},
        .CR3 = 0,
        .KernelStack = kernel_stack,
        .State = TaskState_Runnable,
        .Priority = priority,
        .TimesliceMillis = TASK_TIMESLICE_MILLIS,
        .PrevTask = nullptr,
        .NextTask = nullptr,
    };

    auto stack_top = reinterpret_cast<u8*>(kernel_stack) + STACK_SIZE;
    stack_top = reinterpret_cast<u8*>(reinterpret_cast<uptr>(stack_top) & ~0xF);

    stack_top -= 8;
    *reinterpret_cast<u64*>(stack_top) = reinterpret_cast<u64>(task_exit);

    task->Frame.rip = reinterpret_cast<u64>(entry);
    task->Frame.rsp = reinterpret_cast<u64>(stack_top);
    task->Frame.rflags = 0x202;
    task->Frame.cs = 0x08;
    task->Frame.ss = 0x10;

    task->Frame.rdi = reinterpret_cast<u64>(arg);

    return task;
}

void task::EnqueueTask(Task* task)
{
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

        it->NextTask = task;
        task->PrevTask = it;
    }
}

task::Task* task::NextTask()
{
    Reaper();

    if (!task_queue_root)
    {
        return nullptr;
    }

    if (!ActiveTask)
    {
        return task_queue_root;
    }

    if (ActiveTask->TimesliceMillis)
    {
        ActiveTask->TimesliceMillis--;
        return ActiveTask;
    }

    if (!ActiveTask->NextTask)
    {
        for (auto task = task_queue_root; task; task = task->NextTask)
        {
            if (task->State == TaskState_Running)
            {
                task->State = TaskState_Runnable;
                task->TimesliceMillis = TASK_TIMESLICE_MILLIS;
            }
        }
        return task_queue_root;
    }

    Task* task;
    for (task = ActiveTask; task && task->State != TaskState_Runnable; task = task->NextTask)
        ;

    return task;
}

void task::Reaper()
{
    for (auto task = task_queue_root; task;)
    {
        if (task->State == TaskState_Zombie)
        {
            if (task == task_queue_root)
            {
                task_queue_root = task->NextTask;
            }

            if (task->PrevTask)
            {
                task->PrevTask->NextTask = task->NextTask;
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
