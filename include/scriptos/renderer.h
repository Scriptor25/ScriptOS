#pragma once

#include <scriptos/framebuffer.h>

template<typename T>
struct Point
{
    T X{}, Y{};
};

class Renderer
{
public:
    Renderer(void* front_buffer,
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
             u8 blue_size);

    u32 RepackColor(u32 color) const;

    void SetForeground(u32 color);
    void SetBackground(u32 color);

    void Clear();
    void Shift(usize up);

    void SwapBuffers();

    usize CursorX() const;
    usize CursorY() const;

    void DrawPixel(usize x,
                   usize y,
                   u32 color);
    void DrawChar(int c,
                  usize x,
                  usize y);

    void Reset();
    void NextChar(int c);

    void NewLine();

private:
    u8 *m_FrontBuffer, *m_BackBuffer;

    usize m_Width, m_Height, m_Pitch;
    usize m_Area, m_Size;

    u32 m_Bitmask;

    bool m_RequireRepack;
    u32 m_RedShift, m_RedMask, m_GreenShift, m_GreenMask, m_BlueShift, m_BlueMask;
    u32 m_Stride;

    bool m_Dirty{};

    u32 m_Foreground{}, m_Background{};

    Point<usize> m_Cursor;
};
