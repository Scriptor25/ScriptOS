#pragma once

#include <scriptos/std/types.hpp>

#define GDT_CODE_SEGMENT 0x08
#define GDT_DATA_SEGMENT 0x10
#define GDT_USER_CODE_SEGMENT 0x18
#define GDT_USER_DATA_SEGMENT 0x20
#define GDT_TASK_STATE_SEGMENT 0x28

namespace GDT
{
    /**
     * Global Descriptor Table - Access Enum
     */
    enum Access
    {
        Access_Code_Readable = 0b00000010,
        Access_Data_Writeable = 0b00000010,

        Access_Code_Conforming = 0b00000100,
        Access_Data_Direction_Down = 0b00000100,

        Access_Task_Segment = 0b00001000,
        Access_Data_Segment = 0b00010000,
        Access_Code_Segment = 0b00011000,

        Access_Ring0 = 0b00000000,
        Access_Ring1 = 0b00100000,
        Access_Ring2 = 0b01000000,
        Access_Ring3 = 0b01100000,

        Access_Present = 0b10000000,
    };

    /**
     * Global Descriptor Table - Flags Enum
     */
    enum Flags
    {
        Flags_64Bit = 0b0010,
        Flags_32Bit = 0b0100,
        Flags_16Bit = 0b0000,

        Flags_Granularity_1B = 0b0000,
        Flags_Granularity_4K = 0b1000,
    };

    /**
     * Global Descriptor Table - Entry
     */
    struct Entry
    {
        Entry() = default;
        Entry(u32 base, u32 limit, u8 access, u8 flags);

        u16 LimitLo;
        u16 BaseLo;
        u8 BaseMi;
        u8 Access;
        u8 LimitHi : 4;
        u8 Flags : 4;
        u8 BaseHi;
    } __attribute__((packed));

    /**
     * Global Descriptor Table - Descriptor
     *
     * Representation for the structure of the contents inside the GDTR.
     */
    struct Descriptor
    {
        u16 Size;
        Entry *Offset;
    } __attribute__((packed));

    /**
     * Initialize the kernel protected mode GDT
     */
    void Initialize(void *kernel_stack);

    /**
     * Load a GDT descriptor into the GDTR
     */
    extern "C" void LoadGDT(const Descriptor *descriptor, u16 code_segment, u16 data_segment);

    /**
     * Load a LDT descriptor into the LDTR
     */
    extern "C" void LoadLDT(const Descriptor *descriptor);
}
