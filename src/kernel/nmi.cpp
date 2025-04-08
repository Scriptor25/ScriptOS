#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/nmi.hpp>

void NMI::Enable()
{
    outb(0x70, inb(0x70) & 0x7F);
    inb(0x71);
}

void NMI::Disable()
{
    outb(0x70, inb(0x70) | 0x80);
    inb(0x71);
}

bool NMI::Get(u8 offset)
{
    auto bits = inb((offset & 0b1000) ? NMI_PORT_B : NMI_PORT_A);
    auto mask = 1 << (offset & 0b111);
    return bits & mask;
}
