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
    extern u8 basic[128][8];

    /**
     * Contains an 8x8 font map for unicode points U+2580 - U+259F (block elements)
     */
    extern u8 block[32][8];

    /**
     * Contains an 8x8 font map for unicode points U+2500 - U+257F (box drawing)
     */
    extern u8 box[128][8];

    /**
     * Contains an 8x8 font map for unicode points U+0080 - U+009F (C1/C2 control)
     */
    extern u8 control[32][8];

    /**
     * Contains an 8x8 font map for unicode points U+00A0 - U+00FF (extended latin)
     */
    extern u8 ext_latin[96][8];

    /**
     * Contains an 8x8 font map for unicode points U+0390 - U+03C9 (greek characters)
     */
    extern u8 greek[58][8];

    /**
     * Contains an 8x8 font map for unicode points U+3040 - U+309F (Hiragana)
     */
    extern u8 hiragana[96][8];

    /**
     */
    extern u8 misc[10][8];

    /**
     */
    extern u8 sga[26][8];

    const u8* GetChar(int c);

    bool GetBit(const u8* c, u8 x, u8 y);
}
