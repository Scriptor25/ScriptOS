#include <scriptos/kernel/graphics.hpp>
#include <scriptos/std/util.hpp>

Color::Color(u32 color)
    : a(static_cast<f32>((color >> 24) & 0xff) / 255.f),
      r(static_cast<f32>((color >> 16) & 0xff) / 255.f),
      g(static_cast<f32>((color >> 8) & 0xff) / 255.f),
      b(static_cast<f32>(color & 0xff) / 255.f)
{
}

Color::Color(f32 a, f32 r, f32 g, f32 b)
    : a(a), r(r), g(g), b(b)
{
}

Color::operator u32() const
{
    auto ia = static_cast<u32>(a * 255.f);
    auto ir = static_cast<u32>(r * 255.f);
    auto ig = static_cast<u32>(g * 255.f);
    auto ib = static_cast<u32>(b * 255.f);

    return (ia & 0xff) << 24 | (ir & 0xff) << 16 | (ig & 0xff) << 8 | (ib & 0xff);
}

Color operator*(const Color &lhs, f32 rhs)
{
    return {
        lhs.a * rhs,
        lhs.r * rhs,
        lhs.g * rhs,
        lhs.b * rhs,
    };
}

Color operator*(f32 lhs, const Color &rhs)
{
    return rhs * lhs;
}

Color operator+(const Color &lhs, const Color &rhs)
{
    return {
        lhs.a + rhs.a,
        lhs.r + rhs.r,
        lhs.g + rhs.g,
        lhs.b + rhs.b,
    };
}

Graphics &Graphics::GetKernelInstance()
{
    static Graphics instance;
    return instance;
}

u32 Graphics::Blend(u32 src, u32 dst)
{
    Color src_color(src);
    Color dst_color(dst);

    auto src_factor = src_color.a;
    auto dst_factor = 1.f - src_color.a;

    return src_color * src_factor + dst_color * dst_factor;
}

void Graphics::Initialize(u8 *fb_addr, u8 *bb_addr, u32 width, u32 height, u32 pitch, u8 bpp)
{
    m_Dirty = false;

    m_FrontBuffer.Initialize(fb_addr, width, height, pitch, bpp);
    m_BackBuffer.Initialize(bb_addr, width, height, pitch, bpp);
}

u32 Graphics::Width() const
{
    return m_FrontBuffer.Width();
}

u32 Graphics::Height() const
{
    return m_FrontBuffer.Height();
}

Point<usize> &Graphics::Pos()
{
    return m_Pos;
}

const Point<usize> &Graphics::Pos() const
{
    return m_Pos;
}

void Graphics::SwapBuffers()
{
    if (!m_Dirty)
        return;

    Framebuffer::Blit(m_FrontBuffer, m_BackBuffer);
    m_Dirty = false;
}

void Graphics::Clear()
{
    m_BackBuffer.Clear(m_BGColor);
    m_Dirty = true;
}

void Graphics::DrawPixel(usize x, usize y, u32 color)
{
    m_BackBuffer.Write(x, y, color);
    m_Dirty = true;
}

void Graphics::DrawChar(int c, usize x, usize y)
{
    auto bmp = Font::GetChar(c);
    if (!bmp)
        return;

    u32 buffer[8][8];

    for (u8 j = 0; j < 8; ++j)
        for (u8 i = 0; i < 8; ++i)
        {
            auto color = m_BGColor;
            if (Font::GetBit(bmp, i, j))
                color = m_FGColor;
            buffer[j][i] = color;
        }

    DrawTexture(x, y, 0.f, 0.f, x + 8, y + 8, 1.f, 1.f, 8, 8, false, &buffer[0][0]);
}

void Graphics::DrawRect(usize x1, usize y1, usize x2, usize y2)
{
    m_BackBuffer.Fill(x1, y1, x2 - x1 + 1, y2 - y1 + 1, m_FGColor);
    m_Dirty = true;
}

void Graphics::DrawString(usize x, usize y, usize wrap, cstr data)
{
    auto ptr = const_cast<str>(data);
    for (usize dx = 0, dy = 0; *ptr; ++ptr)
    {
        if (!dx && *ptr <= 0x20)
            continue;
        DrawChar(*ptr, x + dx, y + dy);
        dx += 8;
        if (wrap && dx >= wrap - 8)
        {
            if (*(ptr + 1) > 0x20)
                DrawChar('-', x + dx, y + dy);
            dx = 0;
            dy += 8;
        }
    }
}

void Graphics::DrawString(usize x, usize y, usize wrap, cwstr data)
{
    auto ptr = const_cast<wstr>(data);
    for (usize dx = 0, dy = 0; *ptr; ++ptr)
    {
        if (!dx && *ptr <= 0x20)
            continue;
        DrawChar(*ptr, x + dx, y + dy);
        dx += 8;
        if (wrap && dx >= wrap - 8)
        {
            if (*(ptr + 1) > 0x20)
                DrawChar('-', x + dx, y + dy);
            dx = 0;
            dy += 8;
        }
    }
}

void Graphics::DrawTexture(usize x1, usize y1, f32 u1, f32 v1, usize x2, usize y2, f32 u2, f32 v2, usize width, usize height, bool filter, const u32 *data)
{
    for (usize y = y1; y < y2; ++y)
        for (usize x = x1; x < x2; ++x)
        {
            auto tx = static_cast<f32>(x - x1) / static_cast<f32>(x2 - x1);
            auto ty = static_cast<f32>(y - y1) / static_cast<f32>(y2 - y1);

            auto u = (1 - tx) * u1 + tx * u2;
            auto v = (1 - ty) * v1 + ty * v2;

            u32 value;
            if (!filter)
            {
                auto px = static_cast<usize>(u * width) % width;
                auto py = static_cast<usize>(v * height) % height;

                value = data[px + py * width];
            }
            else
            {
                auto px = u * (width - 1);
                auto py = v * (height - 1);

                auto px0 = static_cast<usize>(floor(px)) % width;
                auto py0 = static_cast<usize>(floor(py)) % height;
                auto px1 = static_cast<usize>(ceil(px)) % width;
                auto py1 = static_cast<usize>(ceil(py)) % height;

                Color c00 = data[px0 + py0 * width];
                Color c10 = data[px1 + py0 * width];
                Color c01 = data[px0 + py1 * width];
                Color c11 = data[px1 + py1 * width];

                auto dx = px - px0;
                auto dy = py - py0;

                auto c0 = lerp(c00, c10, dx);
                auto c1 = lerp(c01, c11, dx);

                value = lerp(c0, c1, dy);
            }

            auto color = Blend(value, m_BackBuffer.Read(x, y));
            m_BackBuffer.Write(x, y, color);
        }

    m_Dirty = true;
}

void Graphics::ClearRainbow(usize offset, usize scale)
{
    auto fb_width = m_BackBuffer.Width();
    auto fb_height = m_BackBuffer.Height();

    for (usize j = 0; j < fb_height; ++j)
        for (usize i = 0; i < fb_width; ++i)
        {
            auto fr = static_cast<f32>(((i + offset) * scale) % fb_width) / static_cast<f32>(fb_width - 1);
            auto fg = static_cast<f32>(((j + offset) * scale) % fb_height) / static_cast<f32>(fb_height - 1);

            auto ur = static_cast<u32>(fr * 255.999f);
            auto ug = static_cast<u32>(fg * 255.999f);

            auto color = (ur & 0xff) << 16 | (ug & 0xff) << 8;
            m_BackBuffer.Write(i, j, color);
        }

    m_Dirty = true;
}

void Graphics::Reset()
{
    m_Pos.x = m_Pos.y = 0;
}

void Graphics::PutChar(int c)
{
    auto fbw = Width();
    auto fbh = Height();

    if (c < 0x20)
    {
        switch (c)
        {
        case '\n':
            m_Pos.x = 0;
            if ((m_Pos.y += CHAR_H) >= fbh)
                m_Pos.y = 0;
            break;
        case '\r':
            m_Pos.x = 0;
            break;
        }
        return;
    }

    DrawChar(c, m_Pos.x, m_Pos.y);

    if ((m_Pos.x += CHAR_W) >= fbw)
    {
        m_Pos.x = 0;
        if ((m_Pos.y += CHAR_H) >= fbh)
            m_Pos.y = 0;
    }
}

void Graphics::SetFGColor(u32 color)
{
    m_FGColor = color;
}

void Graphics::SetBGColor(u32 color)
{
    m_BGColor = color;
}
