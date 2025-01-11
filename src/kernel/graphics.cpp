#include <scriptos/graphics.hpp>

Graphics Graphics::INSTANCE;

Graphics &Graphics::Get()
{
    return INSTANCE;
}

void Graphics::Init(u8 *fb_addr, u8 *bb_addr, u32 width, u32 height, u32 pitch, u8 bpp)
{
    m_Dirty = true;

    m_FrontBuffer.Init(fb_addr, width, height, pitch, bpp);
    m_BackBuffer.Init(bb_addr, width, height, pitch, bpp);
}

u32 Graphics::Width() const
{
    return m_FrontBuffer.Width();
}

u32 Graphics::Height() const
{
    return m_FrontBuffer.Height();
}

void Graphics::SwapBuffers()
{
    if (!m_Dirty)
        return;

    m_Dirty = false;

    Framebuffer::Blit(m_FrontBuffer, m_BackBuffer);
}

void Graphics::Clear(u32 color)
{
    m_Dirty = true;

    m_BackBuffer.Clear(color);
}

void Graphics::DrawChar(int c, usize x, usize y, u32 color)
{
    auto bmp = Font_GetChar(c);
    if (!bmp)
        return;

    m_Dirty = true;

    for (u8 j = 0; j < 8; ++j)
        for (u8 i = 0; i < 8; ++i)
            if (Font_GetBit(bmp, i, j))
                m_BackBuffer.Write(x + i, y + j, color);
}

void Graphics::DrawRect(usize x1, usize y1, usize x2, usize y2, u32 color)
{
    m_Dirty = true;

    m_BackBuffer.Fill(x1, y1, x2 - x1, y2 - y1, color);
}

void Graphics::DrawColorTest(usize offset, usize scale)
{
    m_Dirty = true;

    auto fb_width = m_BackBuffer.Width();
    auto fb_height = m_BackBuffer.Height();

    for (usize j = 0; j < fb_height; ++j)
        for (usize i = 0; i < fb_width; ++i)
        {
            auto fr = (f32)(((i + offset) * scale) % fb_width) / (f32)(fb_width - 1);
            auto fg = (f32)(((j + offset) * scale) % fb_height) / (f32)(fb_height - 1);

            auto ur = (u32)(fr * 255.999f);
            auto ug = (u32)(fg * 255.999f);

            auto color = (ur & 0xff) << 16 | (ug & 0xff) << 8;
            m_BackBuffer.Write(i, j, color);
        }
}

void Graphics::Reset()
{
    m_PosX = m_PosY = 0;
}

void Graphics::PutChar(int c)
{
    auto rows = Height() / 12;
    auto cols = Width() / 8;

    if (c < 0x20)
    {
        switch (c)
        {
        case '\n':
            m_PosX = 0;
            if (++m_PosY >= rows)
                m_PosY = 0;
            break;
        case '\r':
            m_PosX = 0;
            break;
        }
        return;
    }

    DrawChar(c, m_PosX * 8, m_PosY * 12, m_CharColor);

    if (++m_PosX >= cols)
    {
        m_PosX = 0;
        if (++m_PosY >= rows)
            m_PosY = 0;
    }
}

void Graphics::SetCharColor(u32 color)
{
    m_CharColor = color;
}
