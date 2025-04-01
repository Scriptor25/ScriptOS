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

struct Color
{
    Color(u32 color);
    Color(f32 a, f32 r, f32 g, f32 b);

    Color operator*(f32 other) const;
    Color operator+(const Color &other) const;

    operator u32() const;

    union
    {
        struct
        {
            f32 a;
            f32 r;
            f32 g;
            f32 b;
        };
        struct
        {
            f32 w;
            f32 x;
            f32 y;
            f32 z;
        };
        f32 argb[4];
        f32 wxyz[4];
        f32 e[4];
    };
};

class Graphics
{
public:
    /**
     * Get the kernel graphics instance
     */
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

    void DrawTexture(usize x1, usize y1, f32 u1, f32 v1, usize x2, usize y2, f32 u2, f32 v2, usize width, usize height, const u32 *data);

    void ClearRainbow(usize offset = 0, usize scale = 1);

    void Reset();
    void PutChar(int c);

    void SetFGColor(u32 color);
    void SetBGColor(u32 color);

    u32 Blend(u32 src, u32 dst);

private:
    bool m_Dirty;

    Framebuffer m_FrontBuffer, m_BackBuffer;
    Point<usize> m_Pos;

    u32 m_FGColor, m_BGColor;
};
