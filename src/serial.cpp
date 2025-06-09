#include <scriptos/io.h>
#include <scriptos/serial.h>

u8 serial::InitializeAll()
{
    u8 status = 0;

    status |= (Initialize(SERIAL_PORT_COM1) & 0x1) << 0x0;
    status |= (Initialize(SERIAL_PORT_COM2) & 0x1) << 0x1;
    status |= (Initialize(SERIAL_PORT_COM3) & 0x1) << 0x2;
    status |= (Initialize(SERIAL_PORT_COM4) & 0x1) << 0x3;
    status |= (Initialize(SERIAL_PORT_COM5) & 0x1) << 0x4;
    status |= (Initialize(SERIAL_PORT_COM6) & 0x1) << 0x5;
    status |= (Initialize(SERIAL_PORT_COM7) & 0x1) << 0x6;
    status |= (Initialize(SERIAL_PORT_COM8) & 0x1) << 0x7;

    return status;
}

bool serial::Initialize(u16 port)
{
    io::OutB(port + 0x1, 0x00);
    io::OutB(port + 0x3, 0x80);
    io::OutB(port + 0x0, 0x03);
    io::OutB(port + 0x1, 0x00);
    io::OutB(port + 0x3, 0x03);
    io::OutB(port + 0x2, 0xC7);
    io::OutB(port + 0x4, 0x0B);
    io::OutB(port + 0x4, 0x1E);
    io::OutB(port + 0x0, 0xAE);

    if (io::InB(port + 0x0) != 0xAE)
        return false;

    io::OutB(port + 0x4, 0x0F);
    return true;
}

bool serial::IsTransmitEmpty(u16 port)
{
    return io::InB(port + 0x5) & 0x20;
}

void serial::Write(u16 port,
                   u8 data)
{
    while (!IsTransmitEmpty(port))
        ;

    io::OutB(port + 0x0, data);
}

bool serial::HasReceived(u16 port)
{
    return io::InB(port + 0x5) & 0x01;
}

u8 serial::Read(u16 port)
{
    while (!HasReceived(port))
        ;

    return io::InB(port + 0x0);
}

void serial::WriteDefault(int data)
{
    Write(SERIAL_PORT_COM1, data);
}
