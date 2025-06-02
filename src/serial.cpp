#include <scriptos/io.h>
#include <scriptos/serial.h>

bool serial::initialize()
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

u8 serial::transmit_empty()
{
    return io::inb(SERIAL_PORT_COM1 + 5) & 0x20;
}

void serial::write(char data)
{
    while (!transmit_empty())
        ;

    io::outb(SERIAL_PORT_COM1, data);
}

void serial::write(cstr data)
{
    for (str ptr = (str) data; *ptr; ++ptr)
        write(*ptr);
}
