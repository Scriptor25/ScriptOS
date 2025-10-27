#pragma once

#include <scriptos/task/task.h>
#include <scriptos/types.h>

namespace task
{
    extern Task* ActiveTask;

    Task* CreateTask(
        cstr name,
        u64 priority,
        void (*entry)(void*),
        void* arg);

    void EnqueueTask(Task* task);

    Task* NextTask();

    void Reaper();
}
