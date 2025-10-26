#pragma once

#include <scriptos/types.h>

#define GDT_ACCESSED             0b00000001
#define GDT_READABLE_WRITEABLE   0b00000010
#define GDT_DIRECTION_CONFORMING 0b00000100
#define GDT_EXECUTABLE           0b00001000
#define GDT_CODE_DATA            0b00010000
#define GDT_LEVEL_0              0b00000000
#define GDT_LEVEL_1              0b00100000
#define GDT_LEVEL_2              0b01000000
#define GDT_LEVEL_3              0b01100000
#define GDT_PRESENT              0b10000000

#define GDT_LONG_MODE        0b0010
#define GDT_32_BIT_SEGMENT   0b0100
#define GDT_PAGE_GRANULARITY 0b1000

namespace gdt
{
    union SegmentDescriptor
    {
        SegmentDescriptor(
            u32 base,
            u32 limit,
            u8 access,
            u8 flags);

        struct
        {
            u32 LimitLo : 16;
            u32 BaseLo  : 24;
            u8 Access   : 8;
            u32 LimitHi : 4;
            u8 Flags    : 4;
            u32 BaseHi  : 8;
        } __attribute__((packed));

        u64 Value;
    };

    union SystemSegmentDescriptor
    {
        SystemSegmentDescriptor(
            u64 base,
            u32 limit,
            u8 access,
            u8 flags);

        struct
        {
            u32 LimitLo : 16;
            u64 BaseLo  : 24;
            u8 Access   : 8;
            u32 LimitHi : 4;
            u8 Flags    : 4;
            u64 BaseHi  : 40;
            u32         : 32;
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

    usize Insert(
        void* buffer,
        usize offset,
        const SegmentDescriptor& desc);
    usize Insert(
        void* buffer,
        usize offset,
        const SystemSegmentDescriptor& desc);

    extern "C" void __load_gdt(
        const Descriptor* descriptor,
        u16 code_segment,
        u16 data_segment);
}
