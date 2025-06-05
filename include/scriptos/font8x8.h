#pragma once

#include <scriptos/types.h>

/**
 * 8x8 monochrome bitmap fonts for rendering
 * Author: Daniel Hepper <daniel@hepper.net>
 *
 * License: Public Domain
 *
 * Based on:
 * http://dimensionalrift.homelinux.net/combuster/mos3/?p=viewsource&file=/modules/gfx/font8_8.asm
 **/
namespace font8x8
{
    /**
     * Contains an 8x8 font map for unicode points U+0000 - U+007F (basic latin)
     */
    extern u8 BASIC[128][8];

    /**
     * Contains an 8x8 font map for unicode points U+2580 - U+259F (block elements)
     */
    extern u8 BLOCK[32][8];

    /**
     * Contains an 8x8 font map for unicode points U+2500 - U+257F (box drawing)
     */
    extern u8 BOX[128][8];

    /**
     * Contains an 8x8 font map for unicode points U+0080 - U+009F (C1/C2 control)
     */
    extern u8 CONTROL[32][8];

    /**
     * Contains an 8x8 font map for unicode points U+00A0 - U+00FF (extended latin)
     */
    extern u8 EXT_LATIN[96][8];

    /**
     * Contains an 8x8 font map for unicode points U+0390 - U+03C9 (greek characters)
     */
    extern u8 GREEK[58][8];

    /**
     * Contains an 8x8 font map for unicode points U+3040 - U+309F (Hiragana)
     */
    extern u8 HIRAGANA[96][8];

    /**
     */
    extern u8 MISC[10][8];

    /**
     */
    extern u8 SGA[26][8];

    const u8* GetChar(int c);
    bool GetBit(const u8* c, u8 x, u8 y);
}
