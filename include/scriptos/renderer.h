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
    void Initialize(const Framebuffer& framebuffer);

    void SetForeground(u32 color);
    void SetBackground(u32 color);

    void Clear();
    void DrawChar(int c, usize x, usize y);

    void Reset();
    void NextChar(int c);

private:
    Framebuffer m_Framebuffer;

    u32 m_Foreground, m_Background;

    Point<usize> m_Cursor;
};
