#include <scriptos/tss.h>

extern "C" void __load_tr(u16 segment);

kernel::TaskStateSegment kernel::TSS0;

void kernel::InitializeTss(
    void* stack0,
    void* stack1,
    void* stack2)
{
    TSS0.RSP0 = reinterpret_cast<uptr>(stack0);
    TSS0.RSP1 = reinterpret_cast<uptr>(stack1);
    TSS0.RSP2 = reinterpret_cast<uptr>(stack2);

    TSS0.IST1 = 0;
    TSS0.IST2 = 0;
    TSS0.IST3 = 0;
    TSS0.IST4 = 0;
    TSS0.IST5 = 0;
    TSS0.IST6 = 0;
    TSS0.IST7 = 0;

    TSS0.IOPB = sizeof(kernel::TaskStateSegment);

    // __load_tr(0x28);
}
