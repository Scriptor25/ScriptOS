#pragma once

#include <scriptos/types.h>

namespace idt
{
    union GateDescriptor
    {
        GateDescriptor() = default;
        GateDescriptor(u64 offset, u16 segment_selector, u8 interrupt_stack_table, u8 gate_type, u8 privilege_level);

        struct
        {
            u64 OffsetLo           : 16;
            u16 SegmentSelector    : 16;
            u8 InterruptStackTable : 3;
            u8                     : 5;
            u8 GateType            : 4;
            u8                     : 1;
            u8 PrivilegeLevel      : 2;
            u8 Present             : 1;
            u64 OffsetHi           : 48;
            u32                    : 32;
        } __attribute__((packed));

        struct
        {
            u64 ValueLo;
            u64 ValueHi;
        } __attribute__((packed));
    };

    struct Descriptor
    {
        u16 Size;
        void* Offset;
    } __attribute__((packed));

    void Initialize();

    extern "C" void __load_idt(const Descriptor* descriptor);
}
