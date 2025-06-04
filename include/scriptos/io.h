#pragma once

#include <scriptos/types.h>

namespace io
{
    void outb(u16 port, u8 data);

    u8 inb(u16 port);
}
