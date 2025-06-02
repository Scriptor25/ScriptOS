#pragma once

#include <scriptos/types.h>

#define SERIAL_PORT_COM1 0x3f8

namespace serial
{
    bool initialize();

    u8 transmit_empty();

    void write(char data);
    void write(cstr data);
}
