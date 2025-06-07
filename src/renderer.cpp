#include <scriptos/font8x8.h>
#include <scriptos/memory.h>
#include <scriptos/renderer.h>

void Renderer::Initialize(void* front_buffer, void* back_buffer, usize width, usize height)
{
    m_FrontBuffer = front_buffer;
    m_BackBuffer = back_buffer;
    m_Width = width;
    m_Height = height;
    m_Area = width * height;
    m_Dirty = false;
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
    for (usize i = 0; i < m_Area; ++i)
        reinterpret_cast<u32*>(m_BackBuffer)[i] = m_Background;

    m_Dirty = true;
}

void Renderer::Shift(usize up)
{
    auto area_height = m_Height >= up ? m_Height - up : 0;

    auto dst = reinterpret_cast<u32*>(m_BackBuffer);
    auto end = reinterpret_cast<u32*>(m_BackBuffer) + (area_height * m_Width);
    auto src = reinterpret_cast<const u32*>(m_BackBuffer) + (up * m_Width);

    memory::Copy(dst, src, area_height * m_Width * sizeof(u32));

    for (usize i = 0; i < up * m_Width; ++i)
        end[i] = m_Background;

    m_Dirty = true;
}

void Renderer::SwapBuffers()
{
    memory::Copy(m_FrontBuffer, m_BackBuffer, m_Area * sizeof(u32));
}

usize Renderer::CursorX() const
{
    return m_Cursor.X;
}

usize Renderer::CursorY() const
{
    return m_Cursor.Y;
}

void Renderer::DrawPixel(usize x, usize y, u32 color)
{
    auto base = reinterpret_cast<u32*>(m_BackBuffer);
    base[x + y * m_Width] = color;

    m_Dirty = true;
}

void Renderer::DrawChar(int c, usize x, usize y)
{
    auto bitmap = font8x8::GetChar(c);

    auto base = reinterpret_cast<u32*>(m_BackBuffer);

    for (usize j = 0; j < 8; ++j)
    {
        auto row = base + (y + j) * m_Width;
        for (usize i = 0; i < 8; ++i)
            row[x + i] = font8x8::GetBit(bitmap, i, j) ? m_Foreground : m_Background;
    }

    m_Dirty = true;
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
        NewLine();
        break;

    default:
        DrawChar(c, m_Cursor.X, m_Cursor.Y);

        if ((m_Cursor.X + 8) >= m_Width)
        {
            m_Cursor.X = 0;
            NewLine();
        }
        else
            m_Cursor.X += 8;
        break;
    }
}

void Renderer::NewLine()
{
    if ((m_Cursor.Y + 12) >= m_Height)
        Shift(12);
    else
        m_Cursor.Y += 12;
}
