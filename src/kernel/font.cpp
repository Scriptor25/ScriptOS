#include <scriptos/kernel/font.hpp>
#include <scriptos/font/font8x8.hpp>
#include <scriptos/std/memory.hpp>

const u8 *Font::GetChar(int c)
{
    if (0x0000 <= c && c <= 0x007f)
        return font8x8::basic[c];
    if (0x0080 <= c && c <= 0x009f)
        return font8x8::control[c - 0x0080];
    if (0x00a0 <= c && c <= 0x00ff)
        return font8x8::ext_latin[c - 0x00a0];
    if (0x0390 <= c && c <= 0x03c9)
        return font8x8::greek[c - 0x0390];
    if (0x2500 <= c && c <= 0x257f)
        return font8x8::box[c - 0x2500];
    if (0x2580 <= c && c <= 0x259f)
        return font8x8::block[c - 0x2580];
    if (0x3040 <= c && c <= 0x309f)
        return font8x8::hiragana[c - 0x3040];
    if (0xe541 <= c && c <= 0xe55a)
        return font8x8::sga[c - 0xe541];
    return nullptr;
}

bool Font::GetBit(const u8 *c, u8 x, u8 y)
{
    return (c[y] >> x) & 1;
}
