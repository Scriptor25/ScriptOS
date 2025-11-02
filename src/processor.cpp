#include <scriptos/asm.h>
#include <scriptos/memory.h>
#include <scriptos/processor.h>

void processor::Initialize(u64 cpuid)
{
    auto state = memory::AllocateAligned<ProcessorState>(0x40);

    memory::Fill(state, 0, sizeof(ProcessorState));

    state->CPUID = cpuid;
    state->ActiveTask = nullptr;

    wrmsr(GS_BASE, reinterpret_cast<uptr>(state));
}
