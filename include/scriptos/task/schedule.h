#pragma once

#include <scriptos/task/task.h>
#include <scriptos/types.h>

namespace task
{
    extern Task* CurrentTask;

    Task* Create(
        cstr name,
        u64 priority,
        void (*entry)(void*),
        void* arg);

    void Enqueue(Task* task);

    Task* Schedule();

    void Reaper();
}

extern "C"
{
    void __save_context(task::Registers* regs);
    __attribute__((noreturn)) void __restore_context(task::Registers* regs);
}
