#pragma once

#include <scriptos/types.h>

#define PIT_CHANNEL0 0x40
#define PIT_CHANNEL1 0x41
#define PIT_CHANNEL2 0x42
#define PIT_COMMAND  0x43

#define PIT_FREQUENCY 1193182

namespace pit
{
    void Initialize(u32 frequency);
}
