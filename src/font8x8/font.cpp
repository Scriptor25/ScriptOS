#include <scriptos/font8x8.h>

const u8* font8x8::GetChar(int c)
{
    if (0x0000 <= c && c <= 0x007f)
        return font8x8::BASIC[c];
    if (0x0080 <= c && c <= 0x009f)
        return font8x8::CONTROL[c - 0x0080];
    if (0x00a0 <= c && c <= 0x00ff)
        return font8x8::EXT_LATIN[c - 0x00a0];
    if (0x0390 <= c && c <= 0x03c9)
        return font8x8::GREEK[c - 0x0390];
    if (0x2500 <= c && c <= 0x257f)
        return font8x8::BOX[c - 0x2500];
    if (0x2580 <= c && c <= 0x259f)
        return font8x8::BLOCK[c - 0x2580];
    if (0x3040 <= c && c <= 0x309f)
        return font8x8::HIRAGANA[c - 0x3040];
    if (0xe541 <= c && c <= 0xe55a)
        return font8x8::SGA[c - 0xe541];
    return nullptr;
}

bool font8x8::GetBit(const u8* c, u8 x, u8 y)
{
    return (c[y] >> x) & 1;
}
