#include <scriptos/font8x8.h>
#include <scriptos/memory.h>
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

void Renderer::ShiftFramebuffer(usize up)
{
    auto width = m_Framebuffer.Width;
    auto full_height = m_Framebuffer.Height;

    auto area_height = full_height - up;

    auto dst = reinterpret_cast<u32*>(m_Framebuffer.BaseAddress);
    auto end = reinterpret_cast<u32*>(m_Framebuffer.BaseAddress) + (area_height * width);
    auto src = reinterpret_cast<u32*>(m_Framebuffer.BaseAddress) + (up * width);

    memory::Copy(dst, src, area_height * width * sizeof(u32));

    for (usize i = 0; i < up * width; ++i)
        end[i] = m_Background;
}

void Renderer::DrawChar(int c, usize x, usize y)
{
    auto bitmap = font8x8::GetChar(c);

    auto base = reinterpret_cast<u32*>(m_Framebuffer.BaseAddress);

    for (usize j = 0; j < 8; ++j)
    {
        auto row = base + (y + j) * m_Framebuffer.Width;
        for (usize i = 0; i < 8; ++i)
            row[x + i] = font8x8::GetBit(bitmap, i, j) ? m_Foreground : m_Background;
    }
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
        if ((m_Cursor.Y + 12) >= m_Framebuffer.Height)
            ShiftFramebuffer(12);
        else
            m_Cursor.Y += 12;
        break;

    default:
        DrawChar(c, m_Cursor.X, m_Cursor.Y);

        if ((m_Cursor.X + 8) >= m_Framebuffer.Width)
        {
            m_Cursor.X = 0;
            if ((m_Cursor.Y + 12) >= m_Framebuffer.Height)
                ShiftFramebuffer(12);
            else
                m_Cursor.Y += 12;
        }
        else
            m_Cursor.X += 8;
        break;
    }
}
