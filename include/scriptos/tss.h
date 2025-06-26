#pragma once

#include <scriptos/types.h>

namespace tss
{
    struct TaskStateSegment
    {
        u8 _0[4];

        u64 RSP0;
        u64 RSP1;
        u64 RSP2;

        u8 _1[8];

        u64 IST1;
        u64 IST2;
        u64 IST3;
        u64 IST4;
        u64 IST5;
        u64 IST6;
        u64 IST7;

        u8 _2[10];

        u16 IOPB;
    } __attribute__((packed));

    void Initialize(
        void* stack0,
        void* stack1,
        void* stack2I);

    extern "C" void __load_tr(u16 segment);

    extern TaskStateSegment TSS0;
}
