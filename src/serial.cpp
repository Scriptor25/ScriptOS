#include <scriptos/io.h>
#include <scriptos/serial.h>

bool serial::Initialize()
{
    io::OutB(SERIAL_PORT_COM1 + 1, 0x00);
    io::OutB(SERIAL_PORT_COM1 + 3, 0x80);
    io::OutB(SERIAL_PORT_COM1 + 0, 0x03);
    io::OutB(SERIAL_PORT_COM1 + 1, 0x00);
    io::OutB(SERIAL_PORT_COM1 + 3, 0x03);
    io::OutB(SERIAL_PORT_COM1 + 2, 0xC7);
    io::OutB(SERIAL_PORT_COM1 + 4, 0x0B);
    io::OutB(SERIAL_PORT_COM1 + 4, 0x1E);
    io::OutB(SERIAL_PORT_COM1 + 0, 0xAE);

    if (io::InB(SERIAL_PORT_COM1 + 0) != 0xAE)
        return false;

    io::OutB(SERIAL_PORT_COM1 + 4, 0x0F);
    return true;
}

u8 serial::TransmitEmpty()
{
    return io::InB(SERIAL_PORT_COM1 + 5) & 0x20;
}

void serial::Write(int data)
{
    while (!TransmitEmpty())
        ;

    io::OutB(SERIAL_PORT_COM1, data);
}
