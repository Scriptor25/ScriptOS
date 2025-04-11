#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/pit.hpp>

u32 PIT::TicksSinceBoot = 0;

u8 PIT::C0_inb()
{
    return inb(Port_C0_Data);
}

u8 PIT::C1_inb()
{
    return inb(Port_C1_Data);
}

u8 PIT::C2_inb()
{
    return inb(Port_C2_Data);
}

void PIT::C0_outb(u8 data)
{
    outb(Port_C0_Data, data);
}

void PIT::C1_outb(u8 data)
{
    outb(Port_C1_Data, data);
}

void PIT::C2_outb(u8 data)
{
    outb(Port_C2_Data, data);
}

u16 PIT::C0_inw()
{
    return static_cast<u16>(inb(Port_C0_Data)) | static_cast<u16>(inb(Port_C0_Data)) << 8;
}

u16 PIT::C1_inw()
{
    return static_cast<u16>(inb(Port_C1_Data)) | static_cast<u16>(inb(Port_C1_Data)) << 8;
}

u16 PIT::C2_inw()
{
    return static_cast<u16>(inb(Port_C2_Data)) | static_cast<u16>(inb(Port_C2_Data)) << 8;
}

void PIT::C0_outw(u16 data)
{
    outb(Port_C0_Data, data & 0xff);
    outb(Port_C0_Data, (data >> 8) & 0xff);
}

void PIT::C1_outw(u16 data)
{
    outb(Port_C1_Data, data & 0xff);
    outb(Port_C1_Data, (data >> 8) & 0xff);
}

void PIT::C2_outw(u16 data)
{
    outb(Port_C2_Data, data & 0xff);
    outb(Port_C2_Data, (data >> 8) & 0xff);
}

void PIT::Command(u8 channel, u8 access_mode, u8 operating_mode)
{
    outb(Port_Command, (channel & 0b11) << 6 | (access_mode & 0b11) << 4 | (operating_mode & 0b111) << 1 | 0b0);
}

u16 PIT::Read_C0_Count()
{
    CLI();
    Command(Channel_0, Access_Mode_Latch_Count, 0);
    auto count = C0_inw();
    STI();
    return count;
}

u16 PIT::Read_C1_Count()
{
    CLI();
    Command(Channel_0, Access_Mode_Latch_Count, 0);
    auto count = C1_inw();
    STI();
    return count;
}

u16 PIT::Read_C2_Count()
{
    CLI();
    Command(Channel_0, Access_Mode_Latch_Count, 0);
    auto count = C2_inw();
    STI();
    return count;
}
