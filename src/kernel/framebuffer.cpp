#include <scriptos/framebuffer.hpp>
#include <scriptos/memory.hpp>

void Framebuffer::Blit(Framebuffer &dst, const Framebuffer &src)
{
    if (&dst == &src)
        return;
    if (dst.m_Base == src.m_Base)
        return;
    if (dst.m_Pitch * dst.m_Height != src.m_Pitch * src.m_Height)
        return;
    memcpy(dst.m_Base, src.m_Base, dst.m_Pitch * dst.m_Height);
}

void Framebuffer::Init(u8 *base, u32 width, u32 height, u32 pitch, u8 bpp)
{
    m_Base = base;
    m_Width = width;
    m_Height = height;
    m_Pitch = pitch;
    m_BytePerPixel = bpp / 8;
}

void Framebuffer::Write(u32 x, u32 y, u32 value)
{
    *(u32 *)(m_Base + x * m_BytePerPixel + y * m_Pitch) = value;
}

void Framebuffer::WriteArray(u32 x, u32 y, u32 width, u32 height, const void *src)
{
    auto pitch = width * m_BytePerPixel;
    for (u32 j = 0; j < height; ++j)
        memcpy(m_Base + x * m_BytePerPixel + (j + y) * m_Pitch, (u8 *)src + y * pitch, pitch);
}

u32 Framebuffer::Read(u32 x, u32 y)
{
    return *(u32 *)(m_Base + x * m_BytePerPixel + y * m_Pitch);
}

void *Framebuffer::ReadArray(u32 x, u32 y, u32 width, u32 height, void *dst)
{
    auto pitch = width * m_BytePerPixel;
    for (u32 j = 0; j < height; ++j)
        memcpy((u8 *)dst + y * pitch, m_Base + x * m_BytePerPixel + (j + y) * m_Pitch, pitch);
    return dst;
}

void Framebuffer::Clear(u32 value)
{
    memset(m_Base, ((u64)value) << 32 | ((u64)value), m_Pitch * m_Height);
}

u32 Framebuffer::GetWidth() const
{
    return m_Width;
}

u32 Framebuffer::GetHeight() const
{
    return m_Height;
}

u8 Framebuffer::GetBytePerPixel() const
{
    return m_BytePerPixel;
}
