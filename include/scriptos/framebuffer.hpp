#pragma once

#include <scriptos/types.hpp>

typedef struct framebuffer
{
    u8 *Base;
    u32 Width;
    u32 Height;
    u32 Pitch;
    u8 BPP;
} framebuffer_t, *fb_ref_t;

void Framebuffer_Setup(fb_ref_t fb, u8 *base, u32 width, u32 height, u32 pitch, u8 bpp);
void Framebuffer_Write(fb_ref_t fb, u32 x, u32 y, u32 value);
void Framebuffer_Write_Array(fb_ref_t fb, u32 x, u32 y, u32 width, u32 height, const u32 *value);
u32 Framebuffer_Read(fb_ref_t fb, u32 x, u32 y);
void Framebuffer_Read_Array(fb_ref_t fb, u32 x, u32 y, u32 width, u32 height, u32 *dst);
void Framebuffer_Clear(fb_ref_t fb, u32 value);
void Framebuffer_Blit(fb_ref_t dst, fb_ref_t src);
