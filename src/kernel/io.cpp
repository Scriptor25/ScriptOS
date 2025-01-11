#include <scriptos/io.hpp>

void outb(u16 port, u8 value)
{
    asm volatile("out %0, %1" : : "a"(value), "Nd"(port));
}

u8 inb(u16 port)
{
    u8 value;
    asm volatile("in %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}
