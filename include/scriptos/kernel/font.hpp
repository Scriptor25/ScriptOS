#pragma once

#include <scriptos/std/types.hpp>

namespace Font
{
    /**
     * Get a unicode character representation from the kernel default font
     */
    const u8 *GetChar(int c);

    /**
     * Get a single row-column addressable bit of a unicode character from a 8x8 font
     */
    bool GetBit(const u8 *c, u8 x, u8 y);
}
