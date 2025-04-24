#include <scriptos/kernel/io.hpp>

void outb(u16 port, u8 value)
{
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

void outw(u16 port, u16 value)
{
    asm volatile("outw %0, %1" : : "a"(value), "Nd"(port));
}

void outl(u16 port, u32 value)
{
    asm volatile("outl %0, %1" : : "a"(value), "Nd"(port));
}

u8 inb(u16 port)
{
    u8 value;
    asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

u16 inw(u16 port)
{
    u16 value;
    asm volatile("inw %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

u32 inl(u16 port)
{
    u32 value;
    asm volatile("inl %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

void outsb(u16 port, const void* address, u16 count)
{
    asm volatile("rep outsb" : "+S"(address), "+c"(count) : "d"(port));
}

void outsw(u16 port, const void* address, u16 count)
{
    asm volatile("rep outsw" : "+S"(address), "+c"(count) : "d"(port));
}

void outsl(u16 port, const void* address, u16 count)
{
    asm volatile("rep outsl" : "+S"(address), "+c"(count) : "d"(port));
}

void* insb(u16 port, void* address, u16 count)
{
    asm volatile("rep insb" : "+D"(address), "+c"(count) : "d"(port) : "memory");
    return address;
}

void* insw(u16 port, void* address, u16 count)
{
    asm volatile("rep insw" : "+D"(address), "+c"(count) : "d"(port) : "memory");
    return address;
}

void* insl(u16 port, void* address, u16 count)
{
    asm volatile("rep insl" : "+D"(address), "+c"(count) : "d"(port) : "memory");
    return address;
}

void io_wait()
{
    outb(0x80, 0);
}
