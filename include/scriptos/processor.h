#pragma once

#include <scriptos/asm.h>
#include <scriptos/task.h>

namespace processor
{
    struct State
    {
        u64 CpuId;
        task::Task* ActiveTask;
    } __attribute__((aligned(0x40)));

    constexpr u32 FS_BASE = 0xC0000100;
    constexpr u32 GS_BASE = 0xC0000101;
    constexpr u32 KERNEL_GS_BASE = 0xC0000102;

    void Initialize(u64 cpuid);

    inline State* GetProcessorState()
    {
        State* state;
        asm volatile("movq %%gs:0, %0" : "=r"(state));
        return state;
    }

    inline u64 GetProcessorCPUID()
    {
        u64 cpu_id;
        asm volatile("movq %%gs:%c1, %0"
                     : "=r"(cpu_id)
                     : "i"(offsetof(State, CpuId)));
        return cpu_id;
    }

    inline task::Task* GetProcessorActiveTask()
    {
        task::Task* task;
        asm volatile("movq %%gs:%c1, %0"
                     : "=r"(task)
                     : "i"(offsetof(State, ActiveTask)));
        return task;
    }

    inline void SetProcessorActiveTask(task::Task* task)
    {
        asm volatile("movq %0, %%gs:%c1"
                     :
                     : "r"(task), "i"(offsetof(State, ActiveTask))
                     : "memory");
    }

    inline bool NoProcessorState()
    {
        auto base = rdmsr(GS_BASE);
        return !base;
    }
}
