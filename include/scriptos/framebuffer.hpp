#pragma once

#include <scriptos/types.hpp>

class Framebuffer
{
public:
    static void Blit(Framebuffer &dst, const Framebuffer &src);

    void Init(u8 *base, u32 width, u32 height, u32 pitch, u8 bpp);
    void Write(u32 x, u32 y, u32 value);
    void WriteArray(u32 x, u32 y, u32 width, u32 height, const void *src);
    u32 Read(u32 x, u32 y);
    void *ReadArray(u32 x, u32 y, u32 width, u32 height, void *dst);
    void Clear(u32 value);
    void Fill(u32 x, u32 y, u32 width, u32 height, u32 value);

    u32 GetWidth() const;
    u32 GetHeight() const;
    u8 GetBytePerPixel() const;

private:
    u8 *m_Base;
    u32 m_Width;
    u32 m_Height;
    u32 m_Pitch;
    u8 m_BytePerPixel;
};
