#include <scriptos/kernel/io.hpp>

void outb(u16 port, u8 value) { out(port, value); }

void outw(u16 port, u16 value) { out(port, value); }

void outl(u16 port, u32 value) { out(port, value); }

u8 inb(u16 port) { return in<u8>(port); }

u16 inw(u16 port) { return in<u16>(port); }

u32 inl(u16 port) { return in<u32>(port); }

void io_wait()
{
    outb(0x80, 0);
}
