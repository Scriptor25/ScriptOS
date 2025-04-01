#pragma once

#include <scriptos/std/types.hpp>

/**
 * Get a unicode character representation from the kernel default font
 */
const u8 *Font_GetChar(int c);

/**
 * Get a single row-column addressable bit of a unicode character from the kernel default font
 */
bool Font_GetBit(const u8 *c, u8 x, u8 y);
