#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/serial.hpp>

bool Serial::Initialize()
{
    outb(SERIAL_PORT_COM1 + 1, 0x00);
    outb(SERIAL_PORT_COM1 + 3, 0x80);
    outb(SERIAL_PORT_COM1 + 0, 0x03);
    outb(SERIAL_PORT_COM1 + 1, 0x00);
    outb(SERIAL_PORT_COM1 + 3, 0x03);
    outb(SERIAL_PORT_COM1 + 2, 0xC7);
    outb(SERIAL_PORT_COM1 + 4, 0x0B);
    outb(SERIAL_PORT_COM1 + 4, 0x1E);
    outb(SERIAL_PORT_COM1 + 0, 0xAE);

    if (inb(SERIAL_PORT_COM1 + 0) != 0xAE)
        return false;

    outb(SERIAL_PORT_COM1 + 4, 0x0F);
    return true;
}

int Serial::Received()
{
    return inb(SERIAL_PORT_COM1 + 5) & 1;
}

char Serial::Read()
{
    while (!Received())
        ;

    return inb(SERIAL_PORT_COM1);
}

char Serial::ReadNoBlock()
{
    return inb(SERIAL_PORT_COM1);
}

int Serial::Transmit_Empty()
{
    return inb(SERIAL_PORT_COM1 + 5) & 0x20;
}

void Serial::Write(char a)
{
    while (Transmit_Empty() == 0)
        ;

    outb(SERIAL_PORT_COM1, a);
}

void Serial::Write(cstr string)
{
    if (!string)
        return Write("(null)");
    for (auto p = const_cast<str>(string); *p; ++p)
        Write(*p);
}

void Serial::Write(cstr string, usize count)
{
    if (!string)
        return Write("(null)");
    for (usize i = 0; i < count && string[i]; ++i)
        Write(string[i]);
}
