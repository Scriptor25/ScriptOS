#pragma once

#include <scriptos/std/types.hpp>

class Framebuffer
{
public:
    /**
     * Utility function to blit the contents of a framebuffer onto another one.
     */
    static void Blit(Framebuffer& dst, const Framebuffer& src);

    /**
     * Initialize this framebuffer.
     */
    void Initialize(u8* base, u32 width, u32 height, u32 pitch, u8 bpp);

    /**
     * Write a color value at a row-column addressed pixel.
     */
    void Write(u32 x, u32 y, u32 value);
    /**
     * Write a two-dimensional array of colors starting at a row-column addressed pixel using the specified width and
     * height.
     */
    void WriteArray(u32 x, u32 y, u32 width, u32 height, const void* src);

    /**
     * Read a color value at a row-column addressed pixel.
     */
    u32 Read(u32 x, u32 y);
    /**
     * Read a two-dimensional array of colors starting at a row-column addressed pixel using the specified width and
     * height.
     */
    void* ReadArray(u32 x, u32 y, u32 width, u32 height, void* dst);

    /**
     * Clear the framebuffer using the given color value.
     */
    void Clear(u32 value);
    /**
     * Fill a rectangular area with the given color value.
     */
    void Fill(u32 x, u32 y, u32 width, u32 height, u32 value);

    /**
     * Get the width in pixels.
     */
    u32 Width() const;
    /**
     * Get the height in pixels.
     */
    u32 Height() const;
    /**
     * Get the number of bytes per pixel.
     */
    u8 BPP() const;

private:
    u8* m_Base;
    u32 m_Width;
    u32 m_Height;
    u32 m_Pitch;
    u8 m_BPP;
};
