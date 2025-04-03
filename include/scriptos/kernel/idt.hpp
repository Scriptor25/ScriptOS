#pragma once

#include <scriptos/std/types.hpp>

namespace IDT
{
    /**
     * Interrupt Descriptor Table - Attributes Enum
     */
    enum Attributes
    {
        Attributes_Present = 0b10000000,

        Attributes_Ring0 = 0b00000000,
        Attributes_Ring1 = 0b00100000,
        Attributes_Ring2 = 0b01000000,
        Attributes_Ring3 = 0b01100000,

        Attributes_Task_Gate = 0b0101,
        Attributes_16Bit_Interrupt_Gate = 0b0110,
        Attributes_16Bit_Trap_Gate = 0b0111,
        Attributes_32Bit_Interrupt_Gate = 0b1110,
        Attributes_32Bit_Trap_Gate = 0b1111,
    };

    /**
     * Interrupt Descriptor Table - Entry
     */
    struct Entry
    {
        Entry(u32 offset, u16 selector, u8 attributes);

        u16 OffsetLo;
        u16 Selector;
        u8 Reserved;
        u8 Attributes;
        u16 OffsetHi;
    };

    /**
     * Interrupt Descriptor Table - Descriptor
     */
    struct Descriptor
    {
        u16 Limit;
        Entry *Ptr;
    } __attribute__((packed));

    /**
     * Initialize the kernel IDT
     */
    void Initialize();

    /**
     * Load an IDT from a descriptor
     */
    void lidt(const Descriptor &descriptor);
}
