#pragma once

#include <scriptos/types.h>

#define SERIAL_PORT_COM1 0x3F8
#define SERIAL_PORT_COM2 0x2F8
#define SERIAL_PORT_COM3 0x3E8
#define SERIAL_PORT_COM4 0x2E8
#define SERIAL_PORT_COM5 0x5F8
#define SERIAL_PORT_COM6 0x4F8
#define SERIAL_PORT_COM7 0x5E8
#define SERIAL_PORT_COM8 0x4E8

namespace serial
{
    u8 InitializeAll();

    bool Initialize(u16 port);

    bool IsTransmitEmpty(u16 port);

    void Write(u16 port,
               u8 data);

    bool HasReceived(u16 port);

    u8 Read(u16 port);

    void WriteDefault(int data);
}
