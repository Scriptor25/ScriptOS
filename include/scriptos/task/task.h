#pragma once

#include <scriptos/types.h>

#define TASK_NAME_LEN         16
#define TASK_TIMESLICE_MILLIS 10

namespace task
{
    struct Registers
    {
        u64 r15, r14, r13, r12, r11, r10, r9, r8;
        u64 rsi, rdi, rbp, rdx, rcx, rbx, rax;
        u64 rip, cs, rflags, rsp, ss;
    };

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
        char Name[TASK_NAME_LEN];
        Registers Regs;
        u64 CR3;
        void* KernelStack;
        void* FxState;
        TaskState State;
        u64 Priority;
        u64 TimesliceMillis;
        Task* PrevTask;
        Task* NextTask;
    };
}
