#pragma once

#include <scriptos/types.h>

constexpr u16 SERIAL_PORT_COM1 = 0x03F8;
constexpr u16 SERIAL_PORT_COM2 = 0x02F8;
constexpr u16 SERIAL_PORT_COM3 = 0x03E8;
constexpr u16 SERIAL_PORT_COM4 = 0x02E8;
constexpr u16 SERIAL_PORT_COM5 = 0x05F8;
constexpr u16 SERIAL_PORT_COM6 = 0x04F8;
constexpr u16 SERIAL_PORT_COM7 = 0x05E8;
constexpr u16 SERIAL_PORT_COM8 = 0x04E8;

namespace kernel
{
    u8 InitializeAllSerial();

    bool InitializeSerial(u16 port);

    bool IsSerialTransmitEmpty(u16 port);

    void WriteSerial(
        u16 port,
        u8 data);

    bool HasSerialReceived(u16 port);

    u8 ReadSerial(u16 port);

    void WriteSerialDefault(int data);
}
