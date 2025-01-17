#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/serial.hpp>

int Serial_Init()
{
    out<u8>(SERIAL_PORT_COM1 + 1, 0x00); // Disable all interrupts
    out<u8>(SERIAL_PORT_COM1 + 3, 0x80); // Enable DLAB (set baud rate divisor)
    out<u8>(SERIAL_PORT_COM1 + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    out<u8>(SERIAL_PORT_COM1 + 1, 0x00); //                  (hi byte)
    out<u8>(SERIAL_PORT_COM1 + 3, 0x03); // 8 bits, no parity, one stop bit
    out<u8>(SERIAL_PORT_COM1 + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    out<u8>(SERIAL_PORT_COM1 + 4, 0x0B); // IRQs enabled, RTS/DSR set
    out<u8>(SERIAL_PORT_COM1 + 4, 0x1E); // Set in loopback mode, test the serial chip
    out<u8>(SERIAL_PORT_COM1 + 0, 0xAE); // Test serial chip (send byte 0xAE and check if serial returns same byte)

    // Check if serial is faulty (i.e: not same byte as sent)
    if (in<u8>(SERIAL_PORT_COM1 + 0) != 0xAE)
        return 1;

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
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
    for (auto p = (str)string; *p; ++p)
        Serial_Write(*p);
}
