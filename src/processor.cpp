#include <scriptos/asm.h>
#include <scriptos/memory.h>
#include <scriptos/processor.h>

void processor::Initialize(u64 cpu_id)
{
    auto state = memory::AllocateAligned<State>(0x40);

    memory::Fill(state, 0, sizeof(State));

    state->CpuId = cpu_id;
    state->ActiveTask = nullptr;

    wrmsr(GS_BASE, reinterpret_cast<uptr>(state));
}
