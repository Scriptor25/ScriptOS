#pragma once

#include <scriptos/interrupt.h>
#include <scriptos/types.h>

#define TASK_TIMESLICE 10

namespace task
{
    enum TaskState
    {
        TaskState_Runnable,
        TaskState_Running,
        TaskState_Blocked,
        TaskState_Zombie,
    };

    struct Task
    {
        u64 PID;
        cstr Name;
        interrupt::StackFrame Frame;
        void* Stack;
        TaskState State;
        u64 Priority;
        u64 Timeslice;
        Task* PrevTask;
        Task* NextTask;
    };
}
