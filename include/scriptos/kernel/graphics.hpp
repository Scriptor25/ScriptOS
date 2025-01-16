#pragma once

#include <scriptos/kernel/font.hpp>
#include <scriptos/kernel/framebuffer.hpp>

#define CHAR_W 8
#define CHAR_H 12

template <typename T>
struct Point
{
    T x, y;
};

class Graphics
{
public:
    static Graphics &GetInstance();

    void Init(u8 *fb_addr, u8 *bb_addr, u32 width, u32 height, u32 pitch, u8 bpp);
    u32 Width() const;
    u32 Height() const;

    Point<usize> &Pos();
    const Point<usize> &Pos() const;

    void SwapBuffers();

    void Clear();

    void DrawPixel(usize x, usize y, u32 color);

    void DrawChar(int c, usize x, usize y);
    void DrawRect(usize x1, usize y1, usize x2, usize y2);

    void ClearRainbow(usize offset = 0, usize scale = 1);

    void Reset();
    void PutChar(int c);

    void SetFGColor(u32 color);
    void SetBGColor(u32 color);

private:
    bool m_Dirty;

    Framebuffer m_FrontBuffer, m_BackBuffer;
    Point<usize> m_Pos;

    u32 m_FGColor, m_BGColor;
};
