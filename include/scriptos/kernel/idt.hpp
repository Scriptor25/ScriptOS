#pragma once

#include <scriptos/std/types.hpp>

enum IDT_Attributes
{
    IDT_Attributes_Present = 0b10000000,

    IDT_Attributes_Ring0 = 0b00000000,
    IDT_Attributes_Ring1 = 0b00100000,
    IDT_Attributes_Ring2 = 0b01000000,
    IDT_Attributes_Ring3 = 0b01100000,

    IDT_Attributes_Task_Gate = 0b0101,
    IDT_Attributes_16Bit_Interrupt_Gate = 0b0110,
    IDT_Attributes_16Bit_Trap_Gate = 0b0111,
    IDT_Attributes_32Bit_Interrupt_Gate = 0b1110,
    IDT_Attributes_32Bit_Trap_Gate = 0b1111,
};

struct IDT_Entry
{
    IDT_Entry(u32 offset, u16 selector, u8 attributes);

    u16 OffsetLo;
    u16 Selector;
    u8 Reserved;
    u8 Attributes;
    u16 OffsetHi;
};

struct IDT_Descriptor
{
    u16 Limit;
    IDT_Entry *Ptr;
} __attribute__((packed));

void InitIDT();
