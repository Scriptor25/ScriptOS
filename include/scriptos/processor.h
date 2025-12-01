#pragma once

#include <scriptos/asm.h>
#include <scriptos/task.h>

namespace kernel
{
    struct ProcessorState
    {
        u64 CpuId;
        Task* ActiveTask;
    } __attribute__((aligned(0x40)));

    constexpr u32 PROCESSOR_FS_BASE        = 0xC0000100;
    constexpr u32 PROCESSOR_GS_BASE        = 0xC0000101;
    constexpr u32 PROCESSOR_KERNEL_GS_BASE = 0xC0000102;

    void InitializeProcessorState(u64 cpuid);

    inline ProcessorState* GetProcessorState()
    {
        ProcessorState* state;
        asm volatile("movq %%gs:0, %0" : "=r"(state));
        return state;
    }

    inline u64 GetProcessorCpuId()
    {
        u64 cpu_id;
        asm volatile("movq %%gs:%c1, %0"
                     : "=r"(cpu_id)
                     : "i"(offsetof(ProcessorState, CpuId)));
        return cpu_id;
    }

    inline Task* GetProcessorActiveTask()
    {
        Task* task;
        asm volatile("movq %%gs:%c1, %0"
                     : "=r"(task)
                     : "i"(offsetof(ProcessorState, ActiveTask)));
        return task;
    }

    inline void SetProcessorActiveTask(Task* task)
    {
        asm volatile("movq %0, %%gs:%c1"
                     :
                     : "r"(task), "i"(offsetof(ProcessorState, ActiveTask))
                     : "memory");
    }

    inline bool NoProcessorState()
    {
        auto base = rdmsr(PROCESSOR_GS_BASE);
        return !base;
    }
}
