#include <scriptos/io.h>
#include <scriptos/pit.h>

void pit::Initialize(u32 frequency)
{
    auto divisor = static_cast<u16>(PIT_FREQUENCY / frequency);

    io::outb(PIT_COMMAND, 0x36); // channel 0, access mode lo/hi, mode 2, binary

    io::outb(PIT_CHANNEL0, static_cast<u8>((divisor >> 0x00) & 0xFF));
    io::outb(PIT_CHANNEL0, static_cast<u8>((divisor >> 0x08) & 0xFF));
}
