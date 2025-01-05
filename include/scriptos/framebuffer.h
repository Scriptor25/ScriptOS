#pragma once

#include <scriptos/types.h>

typedef struct Framebuffer
{
    void *Base;
    u32 Width;
    u32 Height;
    u32 Pitch;
    u8 BPP;
} Framebuffer_t, *FB_Ref_t;

void Framebuffer_Setup(FB_Ref_t fb, void *base, u32 width, u32 height, u32 pitch, u8 bpp);
void Framebuffer_Write(FB_Ref_t fb, u32 x, u32 y, u32 value);
void Framebuffer_Write_Array(FB_Ref_t fb, u32 x, u32 y, u32 width, u32 height, const u32 *value);
void Framebuffer_Clear(FB_Ref_t fb, u32 value);
void Framebuffer_Blit(FB_Ref_t dst, FB_Ref_t src);
