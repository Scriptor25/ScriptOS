#pragma once

#include <scriptos/std/types.hpp>

#define PIT_BASE_FREQUENCY 0x001234DE
#define PIT_TARGET_FREQUENCY 1000

#if PIT_TARGET_FREQUENCY <= 18

#define PIT_DIVIDER 0
#define PIT_TICKS_PER_SECOND 18

#else

#define PIT_DIVIDER (PIT_BASE_FREQUENCY / PIT_TARGET_FREQUENCY)
#define PIT_TICKS_PER_SECOND PIT_TARGET_FREQUENCY

#endif

namespace PIT
{
    enum Port
    {
        Port_C0_Data = 0x40,
        Port_C1_Data = 0x41,
        Port_C2_Data = 0x42,
        Port_Command = 0x43,
    };

    enum Channel
    {
        Channel_0 = 0b00,
        Channel_1 = 0b01,
        Channel_2 = 0b10,
        Channel_Read_Back = 0b11,
    };

    enum Access_Mode
    {
        Access_Mode_Latch_Count = 0b00,
        Access_Mode_Lo_Byte_Only = 0b01,
        Access_Mode_Hi_Byte_Only = 0b10,
        Access_Mode_Lo_Hi_Byte = 0b11,
    };

    enum Operating_Mode
    {
        Operating_Mode_Interrupt = 0b000,
        Operating_Mode_One_Shot = 0b001,
        Operating_Mode_Rate_Generator = 0b010,
        Operating_Mode_Square_Wave = 0b011,
        Operating_Mode_Software_Strobe = 0b100,
        Operating_Mode_Hardware_Strobe = 0b101,
    };

    extern u32 TicksSinceBoot;

    /**
     * Read a single byte from port C0.
     */
    u8 Read_C0_b();
    /**
     * Read a single byte from port C1.
     */
    u8 Read_C1_b();
    /**
     * Read a single byte from port C2.
     */
    u8 Read_C2_b();

    /**
     * Write a single byte to port C0.
     */
    void Write_C0_b(u8 data);
    /**
     * Write a single byte to port C1.
     */
    void Write_C1_b(u8 data);
    /**
     * Write a single byte to port C2.
     */
    void Write_C2_b(u8 data);

    /**
     * Read a two-byte word from port C0.
     */
    u16 Read_C0_w();
    /**
     * Read a two-byte word from port C1.
     */
    u16 Read_C1_w();
    /**
     * Read a two-byte word from port C2.
     */
    u16 Read_C2_w();

    /**
     * Write a two-byte word to port C0.
     */
    void Write_C0_w(u16 data);
    /**
     * Write a two-byte word to port C1.
     */
    void Write_C1_w(u16 data);
    /**
     * Write a two-byte word to port C2.
     */
    void Write_C2_w(u16 data);

    /**
     * Send a command to the PIT.
     */
    void Command(u8 channel, u8 access_mode, u8 operating_mode);

    /**
     * Read the current value as a two-byte word from port C0.
     */
    u16 Read_C0_Count();
    /**
     * Read the current value as a two-byte word from port C1.
     */
    u16 Read_C1_Count();
    /**
     * Read the current value as a two-byte word from port C2.
     */
    u16 Read_C2_Count();
}
