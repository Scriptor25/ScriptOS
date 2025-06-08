#include <scriptos/font8x8.h>
#include <scriptos/memory.h>
#include <scriptos/renderer.h>

void Renderer::Initialize(void* front_buffer, void* back_buffer, usize width, usize height, usize pitch, usize bpp)
{
    m_FrontBuffer = reinterpret_cast<u8*>(front_buffer);
    m_BackBuffer = reinterpret_cast<u8*>(back_buffer);

    m_Width = width;
    m_Height = height;
    m_Pitch = pitch;
    m_BPP = bpp;

    m_Area = width * height;
    m_Size = pitch * height;

    m_Bitmask = (1ul << bpp) - 1;

    m_Stride = pitch / width;

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
    auto color = m_Background & m_Bitmask;

    memory::Fill(m_BackBuffer, 0, m_Size);

    for (usize i = 0; i < m_Size; i += m_Stride)
        *reinterpret_cast<u32*>(m_BackBuffer + i) |= color;

    m_Dirty = true;
}

void Renderer::Shift(usize up)
{
    auto area_height = m_Height >= up ? m_Height - up : 0;
    auto offset = up * m_Pitch;
    auto area_size = area_height * m_Pitch;

    memory::Copy(m_BackBuffer, m_BackBuffer + offset, area_size);

    auto end = m_BackBuffer + area_size;

    auto color = m_Background & m_Bitmask;

    memory::Fill(end, 0, offset);

    for (usize i = 0; i < offset; i += m_Stride)
        *reinterpret_cast<u32*>(end + i) |= color;

    m_Dirty = true;
}

void Renderer::SwapBuffers()
{
    memory::Copy(m_FrontBuffer, m_BackBuffer, m_Size);
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
    auto dst = reinterpret_cast<u32*>(m_BackBuffer + x * m_Stride + y * m_Pitch);

    *dst &= ~m_Bitmask;
    *dst |= (color & m_Bitmask);

    m_Dirty = true;
}

void Renderer::DrawChar(int c, usize x, usize y)
{
    auto bitmap = font8x8::GetChar(c);

    for (usize j = 0; j < 8; ++j)
    {
        auto row = m_BackBuffer + (y + j) * m_Pitch;
        for (usize i = 0; i < 8; ++i)
        {
            auto dst = reinterpret_cast<u32*>(row + (x + i) * m_Stride);

            *dst &= ~m_Bitmask;
            *dst |= (font8x8::GetBit(bitmap, i, j) ? m_Foreground : m_Background) & m_Bitmask;
        }
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
