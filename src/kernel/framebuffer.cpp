#include <scriptos/framebuffer.hpp>
#include <scriptos/memory.hpp>

void Framebuffer_Setup(fb_ref_t fb, u8 *base, u32 width, u32 height, u32 pitch, u8 bpp)
{
    fb->Base = base;
    fb->Width = width;
    fb->Height = height;
    fb->Pitch = pitch;
    fb->BPP = bpp / 8;
}

void Framebuffer_Write(fb_ref_t fb, u32 x, u32 y, u32 value)
{
    *((u32 *)(fb->Base + x * fb->BPP + y * fb->Pitch)) = value;
}

void Framebuffer_Write_Array(fb_ref_t fb, u32 x, u32 y, u32 width, u32 height, const u32 *value)
{
    for (u32 j = 0; j < height; ++j)
        memcpy(fb->Base + x * fb->BPP + (j + y) * fb->Pitch, value + y * width, width);
}

u32 Framebuffer_Read(fb_ref_t fb, u32 x, u32 y)
{
    return *((u32 *)(fb->Base + x * fb->BPP + y * fb->Pitch));
}

void Framebuffer_Read_Array(fb_ref_t fb, u32 x, u32 y, u32 width, u32 height, u32 *dst)
{
    for (u32 j = 0; j < height; ++j)
        memcpy(dst + y * width, fb->Base + x * fb->BPP + (j + y) * fb->Pitch, width);
}

void Framebuffer_Clear(fb_ref_t fb, u32 value)
{
    memset(fb->Base, ((u64)value) << 32 | ((u64)value), fb->Pitch * fb->Height);
}

void Framebuffer_Blit(fb_ref_t dst, fb_ref_t src)
{
    if (dst == src)
        return;
    if (dst->Base == src->Base)
        return;
    if (dst->Pitch * dst->Height != src->Pitch * src->Height)
        return;
    memcpy(dst->Base, src->Base, dst->Pitch * dst->Height);
}
