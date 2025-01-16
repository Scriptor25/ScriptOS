#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/pit.hpp>

u8 PIT_Read_C0_b()
{
    return in<u8>(PIT_Port_C0_Data);
}

u8 PIT_Read_C1_b()
{
    return in<u8>(PIT_Port_C1_Data);
}

u8 PIT_Read_C2_b()
{
    return in<u8>(PIT_Port_C2_Data);
}

void PIT_Write_C0_b(u8 data)
{
    out<u8>(PIT_Port_C0_Data, data);
}

void PIT_Write_C1_b(u8 data)
{
    out<u8>(PIT_Port_C1_Data, data);
}

void PIT_Write_C2_b(u8 data)
{
    out<u8>(PIT_Port_C2_Data, data);
}

u16 PIT_Read_C0_w()
{
    return (u16)in<u8>(PIT_Port_C0_Data) | (u16)in<u8>(PIT_Port_C0_Data) << 8;
}

u16 PIT_Read_C1_w()
{
    return (u16)in<u8>(PIT_Port_C1_Data) | (u16)in<u8>(PIT_Port_C1_Data) << 8;
}

u16 PIT_Read_C2_w()
{
    return (u16)in<u8>(PIT_Port_C2_Data) | (u16)in<u8>(PIT_Port_C2_Data) << 8;
}

void PIT_Write_C0_w(u16 data)
{
    out<u8>(PIT_Port_C0_Data, data & 0xff);
    out<u8>(PIT_Port_C0_Data, (data >> 8) & 0xff);
}

void PIT_Write_C1_w(u16 data)
{
    out<u8>(PIT_Port_C1_Data, data & 0xff);
    out<u8>(PIT_Port_C1_Data, (data >> 8) & 0xff);
}

void PIT_Write_C2_w(u16 data)
{
    out<u8>(PIT_Port_C2_Data, data & 0xff);
    out<u8>(PIT_Port_C2_Data, (data >> 8) & 0xff);
}

void PIT_Command(u8 channel, u8 access_mode, u8 operating_mode)
{
    out<u8>(PIT_Port_Command, (channel & 0b11) << 6 | (access_mode & 0b11) << 4 | (operating_mode & 0b111) << 1 | 0b0);
}

u16 PIT_Read_C0_Count()
{
    CLI();
    PIT_Command(PIT_Channel_0, PIT_Access_Mode_Latch_Count, 0);
    auto count = PIT_Read_C0_w();
    STI();
    return count;
}

u16 PIT_Read_C1_Count()
{
    CLI();
    PIT_Command(PIT_Channel_0, PIT_Access_Mode_Latch_Count, 0);
    auto count = PIT_Read_C1_w();
    STI();
    return count;
}

u16 PIT_Read_C2_Count()
{
    CLI();
    PIT_Command(PIT_Channel_0, PIT_Access_Mode_Latch_Count, 0);
    auto count = PIT_Read_C2_w();
    STI();
    return count;
}
