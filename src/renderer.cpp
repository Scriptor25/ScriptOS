#include <scriptos/font8x8.h>
#include <scriptos/memory.h>
#include <scriptos/renderer.h>

Renderer::Renderer(void* front_buffer,
                   void* back_buffer,
                   usize width,
                   usize height,
                   usize pitch,
                   u16 bpp,
                   u8 red_shift,
                   u8 red_size,
                   u8 green_shift,
                   u8 green_size,
                   u8 blue_shift,
                   u8 blue_size)
    : m_FrontBuffer(reinterpret_cast<u8*>(front_buffer)),
      m_BackBuffer(reinterpret_cast<u8*>(back_buffer)),
      m_Width(width),
      m_Height(height),
      m_Pitch(pitch),
      m_Area(width * height),
      m_Size(pitch * height),
      m_Bitmask((1u << bpp) - 1u),
      m_RequireRepack(red_shift != 16u || red_size != 8u || green_shift != 8u || green_size != 8u || blue_shift != 0u || blue_size != 8u),
      m_RedShift(red_shift),
      m_RedMask((1u << red_size) - 1u),
      m_GreenShift(green_shift),
      m_GreenMask((1u << green_size) - 1u),
      m_BlueShift(blue_shift),
      m_BlueMask((1u << blue_size) - 1u),
      m_Stride(pitch / width)
{
}

u32 Renderer::RepackColor(u32 color) const
{
    if (!m_RequireRepack)
        return color;

    auto r = (color >> 16) & 0xff;
    auto g = (color >> 8) & 0xff;
    auto b = color & 0xff;

    auto rf = static_cast<float>(r) / 255;
    auto gf = static_cast<float>(g) / 255;
    auto bf = static_cast<float>(b) / 255;

    auto rm = static_cast<u32>(rf * static_cast<float>(m_RedMask));
    auto gm = static_cast<u32>(gf * static_cast<float>(m_GreenMask));
    auto bm = static_cast<u32>(bf * static_cast<float>(m_BlueMask));

    return ((rm & m_RedMask) << m_RedShift) | ((gm & m_GreenMask) << m_GreenShift) | ((bm & m_BlueMask) << m_BlueShift);
}

void Renderer::SetForeground(u32 color)
{
    m_Foreground = RepackColor(color);
}

void Renderer::SetBackground(u32 color)
{
    m_Background = RepackColor(color);
}

void Renderer::Clear()
{
    memory::Fill(m_BackBuffer, 0, m_Size);

    for (usize i = 0; i < m_Size; i += m_Stride)
        *reinterpret_cast<u32*>(m_BackBuffer + i) |= m_Background;

    m_Dirty = true;
}

void Renderer::Shift(usize up)
{
    auto area_height = m_Height >= up ? m_Height - up : 0;
    auto offset = up * m_Pitch;
    auto area_size = area_height * m_Pitch;

    memory::Copy(m_BackBuffer, m_BackBuffer + offset, area_size);

    auto end = m_BackBuffer + area_size;

    memory::Fill(end, 0, offset);

    for (usize i = 0; i < offset; i += m_Stride)
        *reinterpret_cast<u32*>(end + i) |= m_Background;

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

void Renderer::DrawPixel(usize x,
                         usize y,
                         u32 color)
{
    color = RepackColor(color);

    auto dst = reinterpret_cast<u32*>(m_BackBuffer + x * m_Stride + y * m_Pitch);

    *dst &= ~m_Bitmask;
    *dst |= color;

    m_Dirty = true;
}

void Renderer::DrawChar(int c,
                        usize x,
                        usize y)
{
    auto bitmap = font8x8::GetChar(c);

    for (usize j = 0; j < 8; ++j)
    {
        auto row = m_BackBuffer + (y + j) * m_Pitch;
        for (usize i = 0; i < 8; ++i)
        {
            auto dst = reinterpret_cast<u32*>(row + (x + i) * m_Stride);

            *dst &= ~m_Bitmask;
            *dst |= font8x8::GetBit(bitmap, i, j) ? m_Foreground : m_Background;
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
