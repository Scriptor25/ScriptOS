#include <scriptos/io.h>
#include <scriptos/serial.h>

bool serial::Initialize()
{
    io::outb(SERIAL_PORT_COM1 + 1, 0x00);
    io::outb(SERIAL_PORT_COM1 + 3, 0x80);
    io::outb(SERIAL_PORT_COM1 + 0, 0x03);
    io::outb(SERIAL_PORT_COM1 + 1, 0x00);
    io::outb(SERIAL_PORT_COM1 + 3, 0x03);
    io::outb(SERIAL_PORT_COM1 + 2, 0xC7);
    io::outb(SERIAL_PORT_COM1 + 4, 0x0B);
    io::outb(SERIAL_PORT_COM1 + 4, 0x1E);
    io::outb(SERIAL_PORT_COM1 + 0, 0xAE);

    if (io::inb(SERIAL_PORT_COM1 + 0) != 0xAE)
        return false;

    io::outb(SERIAL_PORT_COM1 + 4, 0x0F);
    return true;
}

u8 serial::TransmitEmpty()
{
    return io::inb(SERIAL_PORT_COM1 + 5) & 0x20;
}

void serial::Write(char data)
{
    while (!TransmitEmpty())
        ;

    io::outb(SERIAL_PORT_COM1, data);
}

void serial::Write(cstr data)
{
    for (str ptr = (str) data; *ptr; ++ptr)
        Write(*ptr);
}
