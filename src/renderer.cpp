#include <scriptos/font8x8.h>
#include <scriptos/renderer.h>

void Renderer::Initialize(const Framebuffer& framebuffer)
{
    m_Framebuffer = framebuffer;
}

void Renderer::SetForeground(u32 color)
{
    m_Foreground = color;
}

void Renderer::SetBackground(u32 color)
{
    m_Background = color;
}

void Renderer::Clear()
{
    for (usize i = 0; i < m_Framebuffer.Width * m_Framebuffer.Height; ++i)
        reinterpret_cast<u32*>(m_Framebuffer.BaseAddress)[i] = m_Background;
}

void Renderer::DrawChar(int c, usize x, usize y)
{
    auto bitmap = font8x8::GetChar(c);
    for (usize j = 0; j < 8; ++j)
        for (usize i = 0; i < 8; ++i)
            reinterpret_cast<u32*>(m_Framebuffer.BaseAddress)[(x + i) + (y + j) * m_Framebuffer.Width] = font8x8::GetBit(bitmap, i, j) ? m_Foreground : m_Background;
}

void Renderer::Reset()
{
    m_Cursor.X = m_Cursor.Y = 0;
}

void Renderer::NextChar(int c)
{
    switch (c)
    {
    case '\r':
        m_Cursor.X = 0;
        break;
    case '\n':
        m_Cursor.Y += 12;
        break;

    default:
        DrawChar(c, m_Cursor.X, m_Cursor.Y);

        if ((m_Cursor.X += 8) >= m_Framebuffer.Width)
        {
            m_Cursor.X = 0;
            if ((m_Cursor.Y += 12) >= m_Framebuffer.Height)
                m_Cursor.Y = 0;
        }
        break;
    }
}
