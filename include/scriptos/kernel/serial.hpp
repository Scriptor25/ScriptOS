#pragma once

#include <scriptos/std/types.hpp>

#define SERIAL_PORT_COM1 0x3f8 // COM1

namespace Serial
{
    /**
     * Initialize serial communication.
     */
    bool Initialize();
    /**
     * Check if something got written to the serial port.
     */
    int Received();
    /**
     * Read a single byte from the serial port.
     */
    char Read();
    /**
     * Read a single byte from the serial port, without blocking execution.
     */
    char ReadNoBlock();
    /**
     * Check if nothing got written to the serial port.
     */
    int Transmit_Empty();
    /**
     * Write a single character to the serial port.
     */
    void Write(char a);
    /**
     * Write a null-terminated string to the serial port.
     */
    void Write(cstr string);
    /**
     * Write a string to the serial port. This function writes to the port until either the count or a null-terminator
     * is reached.
     */
    void Write(cstr string, usize count);
}
