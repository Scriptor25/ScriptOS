#include <scriptos/framebuffer.h>
#include <scriptos/memory.h>

void Framebuffer_Setup(FB_Ref_t fb, void *base, u32 width, u32 height, u32 pitch, u8 bpp)
{
    fb->Base = base;
    fb->Width = width;
    fb->Height = height;
    fb->Pitch = pitch;
    fb->BPP = bpp / 8;
}

void Framebuffer_Write(FB_Ref_t fb, u32 x, u32 y, u32 value)
{
    *((u32 *)(fb->Base + x * fb->BPP + y * fb->Pitch)) = value;
}

void Framebuffer_Write_Array(FB_Ref_t fb, u32 x, u32 y, u32 width, u32 height, const u32 *value)
{
    for (u32 j = 0; j < height; ++j)
        memcpy(fb->Base + x * fb->BPP + (j + y) * fb->Pitch, value + y * width, width);
}

void Framebuffer_Clear(FB_Ref_t fb, u32 value)
{
    memset(fb->Base, ((u64)value) << 32 | ((u64)value), fb->Pitch * fb->Height);
}

void Framebuffer_Blit(FB_Ref_t dst, FB_Ref_t src)
{
    if (dst == src)
        return;
    if (dst->Base == src->Base)
        return;
    if (dst->Pitch * dst->Height != src->Pitch * src->Height)
        return;
    memcpy(dst->Base, src->Base, dst->Pitch * dst->Height);
}
