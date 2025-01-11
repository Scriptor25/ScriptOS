#pragma once

#include <scriptos/font.hpp>
#include <scriptos/framebuffer.hpp>

class Graphics
{
public:
    static Graphics &Get();

    void Init(u8 *fb_addr, u8 *bb_addr, u32 width, u32 height, u32 pitch, u8 bpp);
    u32 Width() const;
    u32 Height() const;

    void SwapBuffers();

    void Clear(u32 color);

    void DrawChar(int c, usize x, usize y, u32 color);
    void DrawRect(usize x1, usize y1, usize x2, usize y2, u32 color);

    void DrawColorTest(usize offset = 0, usize scale = 1);

    void Reset();
    void PutChar(int c);
    void SetCharColor(u32 color);

private:
    Framebuffer m_FrontBuffer, m_BackBuffer;
    usize m_PosX, m_PosY;
    u32 m_CharColor;

    static Graphics INSTANCE;
};
