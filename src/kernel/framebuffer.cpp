#include <scriptos/kernel/framebuffer.hpp>
#include <scriptos/std/assert.hpp>
#include <scriptos/std/memory.hpp>

void Framebuffer::Blit(Framebuffer& dst, const Framebuffer& src)
{
    if (&dst == &src || dst.m_Base == src.m_Base)
        return;
    assert(dst.m_Pitch * dst.m_Height == src.m_Pitch * src.m_Height);
    memcpy(dst.m_Base, src.m_Base, dst.m_Pitch * dst.m_Height);
}

void Framebuffer::Initialize(u8* base, u32 width, u32 height, u32 pitch, u8 bpp)
{
    m_Base = base;
    m_Width = width;
    m_Height = height;
    m_Pitch = pitch;
    m_BPP = bpp / 8;
}

void Framebuffer::Write(u32 x, u32 y, u32 value)
{
    if (x >= m_Width || y >= m_Height)
        return;
    *reinterpret_cast<u32*>(m_Base + x * m_BPP + y * m_Pitch) = value;
}

void Framebuffer::WriteArray(u32 x, u32 y, u32 width, u32 height, const void* src)
{
    if (x >= m_Width || y >= m_Height)
        return;
    if (x + width > m_Width)
        width = m_Width - x;
    if (y + height > m_Height)
        height = m_Height - y;

    auto pitch = width * m_BPP;
    for (u32 j = 0; j < height; ++j)
        memcpy(m_Base + x * m_BPP + (j + y) * m_Pitch, reinterpret_cast<const u8*>(src) + j * pitch, pitch);
}

u32 Framebuffer::Read(u32 x, u32 y)
{
    if (x >= m_Width || y >= m_Height)
        return 0;
    return *reinterpret_cast<u32*>(m_Base + x * m_BPP + y * m_Pitch);
}

void* Framebuffer::ReadArray(u32 x, u32 y, u32 width, u32 height, void* dst)
{
    if (x >= m_Width || y >= m_Height)
        return dst;
    if (x + width > m_Width)
        width = m_Width - x;
    if (y + height > m_Height)
        height = m_Height - y;

    auto pitch = width * m_BPP;
    for (u32 j = 0; j < height; ++j)
        memcpy(reinterpret_cast<u8*>(dst) + y * pitch, m_Base + x * m_BPP + (j + y) * m_Pitch, pitch);
    return dst;
}

void Framebuffer::Clear(u32 value)
{
    memset(m_Base, value, m_BPP, m_Width * m_Height);
}

void Framebuffer::Fill(u32 x, u32 y, u32 width, u32 height, u32 value)
{
    if (x >= m_Width || y >= m_Height)
        return;
    if (x + width > m_Width)
        width = m_Width - x;
    if (y + height > m_Height)
        height = m_Height - y;

    for (u32 j = 0; j < height; ++j)
        memset(m_Base + x * m_BPP + (j + y) * m_Pitch, value, m_BPP, width);
}

u32 Framebuffer::Width() const
{
    return m_Width;
}

u32 Framebuffer::Height() const
{
    return m_Height;
}

u8 Framebuffer::BPP() const
{
    return m_BPP;
}
