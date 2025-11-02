#pragma once

#include <scriptos/asm.h>
#include <scriptos/task/task.h>

namespace processor
{
    struct ProcessorState
    {
        u64 CPUID;
        task::Task* ActiveTask;
    } __attribute__((aligned(0x40)));

    constexpr u32 FS_BASE = 0xC0000100;
    constexpr u32 GS_BASE = 0xC0000101;
    constexpr u32 KERNEL_GS_BASE = 0xC0000102;

    void Initialize(u64 cpuid);

    inline ProcessorState* GetProcessorState()
    {
        ProcessorState* state;
        asm volatile("movq %%gs:0, %0" : "=r"(state));
        return state;
    }

    inline u64 GetProcessorCPUID()
    {
        u64 cpuid;
        asm volatile("movq %%gs:%c1, %0"
                     : "=r"(cpuid)
                     : "i"(offsetof(ProcessorState, CPUID)));
        return cpuid;
    }

    inline task::Task* GetProcessorActiveTask()
    {
        task::Task* task;
        asm volatile("movq %%gs:%c1, %0"
                     : "=r"(task)
                     : "i"(offsetof(ProcessorState, ActiveTask)));
        return task;
    }

    inline void SetProcessorActiveTask(task::Task* task)
    {
        asm volatile("movq %0, %%gs:%c1"
                     :
                     : "r"(task), "i"(offsetof(ProcessorState, ActiveTask))
                     : "memory");
    }

    inline bool NoProcessorState()
    {
        auto base = rdmsr(GS_BASE);
        return !base;
    }
}
