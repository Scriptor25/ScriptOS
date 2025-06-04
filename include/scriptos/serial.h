#pragma once

#include <scriptos/types.h>

#define SERIAL_PORT_COM1 0x3f8

namespace serial
{
    bool Initialize();

    u8 TransmitEmpty();

    void Write(int data);
    void Write(cstr data);
}
