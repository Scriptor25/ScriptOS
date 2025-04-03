#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/pit.hpp>

u8 PIT::Read_C0_b()
{
    return in<u8>(Port_C0_Data);
}

u8 PIT::Read_C1_b()
{
    return in<u8>(Port_C1_Data);
}

u8 PIT::Read_C2_b()
{
    return in<u8>(Port_C2_Data);
}

void PIT::Write_C0_b(u8 data)
{
    out<u8>(Port_C0_Data, data);
}

void PIT::Write_C1_b(u8 data)
{
    out<u8>(Port_C1_Data, data);
}

void PIT::Write_C2_b(u8 data)
{
    out<u8>(Port_C2_Data, data);
}

u16 PIT::Read_C0_w()
{
    return static_cast<u16>(in<u8>(Port_C0_Data)) | static_cast<u16>(in<u8>(Port_C0_Data)) << 8;
}

u16 PIT::Read_C1_w()
{
    return static_cast<u16>(in<u8>(Port_C1_Data)) | static_cast<u16>(in<u8>(Port_C1_Data)) << 8;
}

u16 PIT::Read_C2_w()
{
    return static_cast<u16>(in<u8>(Port_C2_Data)) | static_cast<u16>(in<u8>(Port_C2_Data)) << 8;
}

void PIT::Write_C0_w(u16 data)
{
    out<u8>(Port_C0_Data, data & 0xff);
    out<u8>(Port_C0_Data, (data >> 8) & 0xff);
}

void PIT::Write_C1_w(u16 data)
{
    out<u8>(Port_C1_Data, data & 0xff);
    out<u8>(Port_C1_Data, (data >> 8) & 0xff);
}

void PIT::Write_C2_w(u16 data)
{
    out<u8>(Port_C2_Data, data & 0xff);
    out<u8>(Port_C2_Data, (data >> 8) & 0xff);
}

void PIT::Command(u8 channel, u8 access_mode, u8 operating_mode)
{
    out<u8>(Port_Command, (channel & 0b11) << 6 | (access_mode & 0b11) << 4 | (operating_mode & 0b111) << 1 | 0b0);
}

u16 PIT::Read_C0_Count()
{
    CLI();
    Command(Channel_0, Access_Mode_Latch_Count, 0);
    auto count = Read_C0_w();
    STI();
    return count;
}

u16 PIT::Read_C1_Count()
{
    CLI();
    Command(Channel_0, Access_Mode_Latch_Count, 0);
    auto count = Read_C1_w();
    STI();
    return count;
}

u16 PIT::Read_C2_Count()
{
    CLI();
    Command(Channel_0, Access_Mode_Latch_Count, 0);
    auto count = Read_C2_w();
    STI();
    return count;
}
