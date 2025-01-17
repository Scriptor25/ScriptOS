#pragma once

#include <scriptos/std/types.hpp>

#define SERIAL_PORT_COM1 0x3f8 // COM1

int Serial_Init();
int Serial_Received();
char Serial_Read();
int Serial_Transmit_Empty();
void Serial_Write(char a);
void Serial_Write(cstr string);
