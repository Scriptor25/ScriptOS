#pragma once

#include <scriptos/types.h>

namespace kernel
{
    constexpr u8 GDT_ACCESSED             = 0b00000001;
    constexpr u8 GDT_READABLE_WRITEABLE   = 0b00000010;
    constexpr u8 GDT_DIRECTION_CONFORMING = 0b00000100;
    constexpr u8 GDT_EXECUTABLE           = 0b00001000;
    constexpr u8 GDT_CODE_DATA            = 0b00010000;
    constexpr u8 GDT_LEVEL_0              = 0b00000000;
    constexpr u8 GDT_LEVEL_1              = 0b00100000;
    constexpr u8 GDT_LEVEL_2              = 0b01000000;
    constexpr u8 GDT_LEVEL_3              = 0b01100000;
    constexpr u8 GDT_PRESENT              = 0b10000000;

    constexpr u8 GDT_LONG_MODE        = 0b0010;
    constexpr u8 GDT_32_BIT_SEGMENT   = 0b0100;
    constexpr u8 GDT_PAGE_GRANULARITY = 0b1000;

    union GlobalSegmentDescriptor
    {
        GlobalSegmentDescriptor(
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

    union GlobalSystemSegmentDescriptor
    {
        GlobalSystemSegmentDescriptor(
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

    struct GlobalDescriptor
    {
        u16 Size;
        void* Offset;
    } __attribute__((packed));

    void InitializeGDT();

    usize InsertGlobalSegmentDescriptor(
        void* buffer,
        usize offset,
        const GlobalSegmentDescriptor& descriptor);
    usize InsertGlobalSystemSegmentDescriptor(
        void* buffer,
        usize offset,
        const GlobalSystemSegmentDescriptor& descriptor);
}
