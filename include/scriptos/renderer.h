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
    void Initialize(void* front_buffer, void* back_buffer, usize width, usize height);

    void SetForeground(u32 color);
    void SetBackground(u32 color);

    void Clear();
    void Shift(usize up);

    void SwapBuffers();

    usize CursorX() const;
    usize CursorY() const;

    void DrawPixel(usize x, usize y, u32 color);
    void DrawChar(int c, usize x, usize y);

    void Reset();
    void NextChar(int c);

    void NewLine();

private:
    void *m_FrontBuffer, *m_BackBuffer;

    usize m_Width, m_Height, m_Area;

    bool m_Dirty;

    u32 m_Foreground, m_Background;

    Point<usize> m_Cursor;
};
