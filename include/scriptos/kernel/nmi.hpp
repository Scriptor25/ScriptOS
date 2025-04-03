#pragma once

#include <scriptos/kernel/io.hpp>

#define NMI_PORT_A 0x92
#define NMI_PORT_B 0x61

#define NMI_ALTERNATE_HOT_RESET 0b0000
#define NMI_ALTERNATE_GATE_A20 0b0001
#define NMI_SECURITY_LOCK 0b0011
#define NMI_WATCHDOG_TIMER_STATUS 0b0100
#define NMI_HDD_2_DRIVE_ACTIVITY 0b0110
#define NMI_HDD_1_DRIVE_ACTIVITY 0b0111

#define NMI_TIMER_2_TIED_TO_SPEAKER 0b1000
#define NMI_SPEAKER_DATA_ENABLE 0b1001
#define NMI_PARITY_CHECK_ENABLE 0b1010
#define NMI_CHANNEL_CHECK_ENABLE 0b1011
#define NMI_REFRESH_REQUEST 0b1100
#define NMI_TIMER_2_OUTPUT 0b1101
#define NMI_CHANNEL_CHECK 0b1110
#define NMI_PARITY_CHECK 0b1111

namespace NMI
{
    /**
     * Enable non-maskable interrupts.
     */
    void Enable();
    /**
     * Disable non-maskable interrupts.
     */
    void Disable();

    /**
     * Get the status at some offset in [0;7]. With the 4th bit set (starting at 1), this will address the offset for control port B.
     */
    bool Get(u8 offset);
}
