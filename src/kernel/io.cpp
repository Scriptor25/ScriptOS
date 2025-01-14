#include <scriptos/kernel/io.hpp>

void outb(u16 port, u8 value)
{
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

void outw(u16 port, u16 value)
{
    asm volatile("outw %0, %1" : : "ax"(value), "Nd"(port));
}

u8 inb(u16 port)
{
    u8 value;
    asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

void io_wait()
{
    outb(0x80, 0);
}
