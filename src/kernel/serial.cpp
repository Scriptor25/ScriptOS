#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/serial.hpp>

int Serial_Init()
{
    out<u8>(SERIAL_PORT_COM1 + 1, 0x00);
    out<u8>(SERIAL_PORT_COM1 + 3, 0x80);
    out<u8>(SERIAL_PORT_COM1 + 0, 0x03);
    out<u8>(SERIAL_PORT_COM1 + 1, 0x00);
    out<u8>(SERIAL_PORT_COM1 + 3, 0x03);
    out<u8>(SERIAL_PORT_COM1 + 2, 0xC7);
    out<u8>(SERIAL_PORT_COM1 + 4, 0x0B);
    out<u8>(SERIAL_PORT_COM1 + 4, 0x1E);
    out<u8>(SERIAL_PORT_COM1 + 0, 0xAE);

    if (in<u8>(SERIAL_PORT_COM1 + 0) != 0xAE)
        return 1;

    out<u8>(SERIAL_PORT_COM1 + 4, 0x0F);
    return 0;
}

int Serial_Received()
{
    return in<u8>(SERIAL_PORT_COM1 + 5) & 1;
}

char Serial_Read()
{
    while (Serial_Received() == 0)
        ;

    return in<u8>(SERIAL_PORT_COM1);
}

int Serial_Transmit_Empty()
{
    return in<u8>(SERIAL_PORT_COM1 + 5) & 0x20;
}

void Serial_Write(char a)
{
    while (Serial_Transmit_Empty() == 0)
        ;

    out<u8>(SERIAL_PORT_COM1, a);
}

void Serial_Write(cstr string)
{
    if (!string)
        return Serial_Write("(null)");
    for (auto p = (str)string; *p; ++p)
        Serial_Write(*p);
}

void Serial_Write(cstr string, usize count)
{
    if (!string)
        return Serial_Write("(null)");
    for (usize i = 0; i < count && string[i]; ++i)
        Serial_Write(string[i]);
}
