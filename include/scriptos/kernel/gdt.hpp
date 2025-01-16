#pragma once

#include <scriptos/std/types.hpp>

#define GDT_CODE_SEGMENT 0x08
#define GDT_DATA_SEGMENT 0x10
#define GDT_USER_CODE_SEGMENT 0x18
#define GDT_USER_DATA_SEGMENT 0x20
#define GDT_TASK_STATE_SEGMENT 0x28

enum GDT_Access
{
    GDT_Access_Code_Readable = 0b00000010,
    GDT_Access_Data_Writeable = 0b00000010,

    GDT_Access_Code_Conforming = 0b00000100,
    GDT_Access_Data_Direction_Down = 0b00000100,

    GDT_Access_Task_Segment = 0b00001000,
    GDT_Access_Data_Segment = 0b00010000,
    GDT_Access_Code_Segment = 0b00011000,

    GDT_Access_Ring0 = 0b00000000,
    GDT_Access_Ring1 = 0b00100000,
    GDT_Access_Ring2 = 0b01000000,
    GDT_Access_Ring3 = 0b01100000,

    GDT_Access_Present = 0b10000000,
};

enum GDT_Flags
{
    GDT_Flags_64Bit = 0b0010,
    GDT_Flags_32Bit = 0b0100,
    GDT_Flags_16Bit = 0b0000,

    GDT_Flags_Granularity_1B = 0b0000,
    GDT_Flags_Granularity_4K = 0b1000,
};

struct GDT_Entry
{
    GDT_Entry() = default;
    GDT_Entry(u32 base, u32 limit, u8 access, u8 flags);

    u16 LimitLo;
    u16 BaseLo;
    u8 BaseMi;
    u8 Access;
    u8 Flags_LimitHi;
    u8 BaseHi;
} __attribute__((packed));

struct GDT_Descriptor
{
    u16 Size;
    GDT_Entry *Ptr;
} __attribute__((packed));

extern "C" void LoadGDT(GDT_Descriptor *descriptor, u16 code_segment, u16 data_segment);

void InitGDT(void *kernel_stack);
