#include <scriptos/io.h>
#include <scriptos/serial.h>

u8 kernel::InitializeAllSerial()
{
    u8 status = 0;

    status |= (InitializeSerial(SERIAL_PORT_COM1) & 0x1) << 0x0;
    status |= (InitializeSerial(SERIAL_PORT_COM2) & 0x1) << 0x1;
    status |= (InitializeSerial(SERIAL_PORT_COM3) & 0x1) << 0x2;
    status |= (InitializeSerial(SERIAL_PORT_COM4) & 0x1) << 0x3;
    status |= (InitializeSerial(SERIAL_PORT_COM5) & 0x1) << 0x4;
    status |= (InitializeSerial(SERIAL_PORT_COM6) & 0x1) << 0x5;
    status |= (InitializeSerial(SERIAL_PORT_COM7) & 0x1) << 0x6;
    status |= (InitializeSerial(SERIAL_PORT_COM8) & 0x1) << 0x7;

    return status;
}

bool kernel::InitializeSerial(u16 port)
{
    io::outb(port + 0x1, 0x00);
    io::outb(port + 0x3, 0x80);
    io::outb(port + 0x0, 0x03);
    io::outb(port + 0x1, 0x00);
    io::outb(port + 0x3, 0x03);
    io::outb(port + 0x2, 0xC7);
    io::outb(port + 0x4, 0x0B);
    io::outb(port + 0x4, 0x1E);
    io::outb(port + 0x0, 0xAE);

    if (io::inb(port + 0x0) != 0xAE)
    {
        return false;
    }

    io::outb(port + 0x4, 0x0F);
    return true;
}

bool kernel::IsSerialTransmitEmpty(u16 port)
{
    return io::inb(port + 0x5) & 0x20;
}

void kernel::WriteSerial(
    u16 port,
    u8 data)
{
    while (!IsSerialTransmitEmpty(port))
        ;

    io::outb(port + 0x0, data);
}

bool kernel::HasSerialReceived(u16 port)
{
    return io::inb(port + 0x5) & 0x01;
}

u8 kernel::ReadSerial(u16 port)
{
    while (!HasSerialReceived(port))
        ;

    return io::inb(port + 0x0);
}

void kernel::WriteSerialDefault(int data)
{
    WriteSerial(SERIAL_PORT_COM1, data);
}
