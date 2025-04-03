#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/nmi.hpp>

void NMI::Enable()
{
    out<u8>(0x70, in<u8>(0x70) & 0x7F);
    in<u8>(0x71);
}

void NMI::Disable()
{
    out<u8>(0x70, in<u8>(0x70) | 0x80);
    in<u8>(0x71);
}

bool NMI::Get(u8 offset)
{
    auto bits = in<u8>((offset & 0b1000) ? NMI_PORT_B : NMI_PORT_A);
    auto mask = 1 << (offset & 0b111);
    return bits & mask;
}
