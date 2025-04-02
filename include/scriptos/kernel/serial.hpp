#pragma once

#include <scriptos/std/types.hpp>

#define SERIAL_PORT_COM1 0x3f8 // COM1

/**
 * Initialize serial communication.
 */
int Serial_Init();
/**
 * Check if something got written to the serial port.
 */
int Serial_Received();
/**
 * Read a single byte from the serial port.
 */
char Serial_Read();
/**
 * Check if nothing got written to the serial port.
 */
int Serial_Transmit_Empty();
/**
 * Write a single character to the serial port.
 */
void Serial_Write(char a);
/**
 * Write a null-terminated string to the serial port.
 */
void Serial_Write(cstr string);
/**
 * Write a string to the serial port. This function writes to the port until either the count or a null-terminator is reached.
 */
void Serial_Write(cstr string, usize count);
