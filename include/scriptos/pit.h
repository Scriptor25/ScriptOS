#pragma once

#include <scriptos/types.h>

constexpr u16 PIT_CHANNEL0 = 0x0040;
constexpr u16 PIT_CHANNEL1 = 0x0041;
constexpr u16 PIT_CHANNEL2 = 0x0042;
constexpr u16 PIT_COMMAND  = 0x0043;

constexpr auto PIT_FREQUENCY = 1193182u;

namespace kernel
{
    void InitializePit(u32 frequency);
}
