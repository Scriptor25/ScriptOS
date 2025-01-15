#pragma once

#include <scriptos/std/types.hpp>

enum PIT_Port
{
    PIT_Port_C0_Data = 0x40,
    PIT_Port_C1_Data = 0x41,
    PIT_Port_C2_Data = 0x42,
    PIT_Port_Command = 0x43,
};

enum PIT_Channel
{
    PIT_Channel_0 = 0b00,
    PIT_Channel_1 = 0b01,
    PIT_Channel_2 = 0b10,
    PIT_Channel_Read_Back = 0b11,
};

enum PIT_Access_Mode
{
    PIT_Access_Mode_Latch_Count = 0b00,
    PIT_Access_Mode_Lo_Byte_Only = 0b01,
    PIT_Access_Mode_Hi_Byte_Only = 0b10,
    PIT_Access_Mode_Lo_Hi_Byte = 0b11,
};

enum PIT_Operating_Mode
{
    PIT_Operating_Mode_Interrupt = 0b000,
    PIT_Operating_Mode_One_Shot = 0b001,
    PIT_Operating_Mode_Rate_Generator = 0b010,
    PIT_Operating_Mode_Square_Wave = 0b011,
    PIT_Operating_Mode_Software_Strobe = 0b100,
    PIT_Operating_Mode_Hardware_Strobe = 0b101,
};

u8 PIT_Read_C0_b();
u8 PIT_Read_C1_b();
u8 PIT_Read_C2_b();

void PIT_Write_C0_b(u8 data);
void PIT_Write_C1_b(u8 data);
void PIT_Write_C2_b(u8 data);

u16 PIT_Read_C0_w();
u16 PIT_Read_C1_w();
u16 PIT_Read_C2_w();

void PIT_Write_C0_w(u16 data);
void PIT_Write_C1_w(u16 data);
void PIT_Write_C2_w(u16 data);

void PIT_Command(u8 channel, u8 access_mode, u8 operating_mode);

u16 PIT_Read_C0_Count();
u16 PIT_Read_C1_Count();
u16 PIT_Read_C2_Count();
