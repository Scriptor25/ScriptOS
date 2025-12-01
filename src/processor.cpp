#include <scriptos/asm.h>
#include <scriptos/memory.h>
#include <scriptos/processor.h>

void kernel::InitializeProcessorState(u64 cpu_id)
{
    auto state = kernel::AllocateAligned<ProcessorState>(0x40);

    kernel::Fill(state, 0, sizeof(ProcessorState));

    state->CpuId      = cpu_id;
    state->ActiveTask = nullptr;

    wrmsr(PROCESSOR_GS_BASE, reinterpret_cast<uptr>(state));
}
