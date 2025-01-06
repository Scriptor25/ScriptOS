#include <scriptos/bitmap.hpp>
#include <scriptos/font.hpp>
#include <scriptos/framebuffer.hpp>
#include <scriptos/info.hpp>
#include <scriptos/io.hpp>
#include <scriptos/memory.hpp>
#include <scriptos/multiboot2.hpp>
#include <scriptos/pfa.hpp>
#include <scriptos/print.hpp>
#include <scriptos/types.hpp>

static u32 posx;
static u32 posy;
static Framebuffer fb;

void reset()
{
    posx = posy = 0;
    fb.Clear(0);
}

void draw_char(int c, u32 x, u32 y, u32 color)
{
    auto bmp = Font_GetChar(c);
    if (!bmp)
        return;
    for (u8 j = 0; j < 8; ++j)
        for (u8 i = 0; i < 8; ++i)
            if (Font_GetBit(bmp, i, j))
                fb.Write(x + i, y + j, color);
}

void draw_test(int offset)
{
    for (u32 j = 0; j < fb.GetHeight(); ++j)
        for (u32 i = 0; i < fb.GetWidth(); ++i)
        {
            f32 fr = (f32)(((i + offset) * 8) % fb.GetWidth()) / (f32)(fb.GetWidth() - 1);
            f32 fg = (f32)(((j + offset) * 8) % fb.GetHeight()) / (f32)(fb.GetHeight() - 1);

            u32 ur = (u32)(fr * 255.999f);
            u32 ug = (u32)(fg * 255.999f);

            u32 color = (ur & 0xff) << 16 | (ug & 0xff) << 8;
            fb.Write(i, j, color);
        }
}

void putchar(int c)
{
    u32 rows = (fb.GetBytePerPixel() == 2) ? fb.GetHeight() : (fb.GetHeight() / 8);
    u32 cols = (fb.GetBytePerPixel() == 2) ? fb.GetWidth() : (fb.GetWidth() / 8);

    if (c < 0x20)
    {
        switch (c)
        {
        case '\n':
            posx = 0;
            if (++posy >= rows)
                posy = 0;
            break;
        case '\r':
            posx = 0;
            break;
        }
        return;
    }

    if (fb.GetBytePerPixel() == 2)
        fb.Write(posx, posy, 7 << 8 | (c & 0xff));
    else if (fb.GetBytePerPixel() == 3 || fb.GetBytePerPixel() == 4)
        draw_char(c, posx * 8, posy * 8, 0xffffff);

    if (++posx >= cols)
    {
        posx = 0;
        if (++posy >= rows)
            posy = 0;
    }
}

void wprint(const int *str)
{
    for (int *p = (int *)str; *p; ++p)
        putchar(*p);
}

mb_tag_t *get_tag(mb_tag_t *info, u32 type)
{
    for (mb_tag_t *ptr = info + 1;
         ptr->type != MULTIBOOT_TAG_TYPE_END;
         ptr = (mb_tag_t *)((u8 *)ptr + ((ptr->size + 7) & ~7)))
        if (ptr->type == type)
            return ptr;
    return nullptr;
}

extern "C" void kernel_main(u32 magic, mb_tag_t *info)
{
    if ((magic != MULTIBOOT2_BOOTLOADER_MAGIC) || ((u32)info & 7))
        return;

    {
        mb_tag_framebuffer_t *tag = (mb_tag_framebuffer_t *)get_tag(info, MULTIBOOT_TAG_TYPE_FRAMEBUFFER);

        u64 fb_addr = tag->framebuffer_addr;
        u32 fb_width = tag->framebuffer_width;
        u32 fb_height = tag->framebuffer_height;
        u32 fb_pitch = tag->framebuffer_pitch;
        u8 fb_bpp = tag->framebuffer_bpp;

        fb.Init((u8 *)fb_addr, fb_width, fb_height, fb_pitch, fb_bpp);
        reset();
    }

    PageFrameAllocator alloc;
    {
        mb_tag_mmap_t *tag = (mb_tag_mmap_t *)get_tag(info, MULTIBOOT_TAG_TYPE_MMAP);
        const MemoryMap mmap(tag->entries, (mb_mmap_entry_t *)((u8 *)tag + tag->size), tag->entry_size);
        alloc.Init(mmap);
    }

    for (mb_tag_t *ptr = info + 1;
         ptr->type != MULTIBOOT_TAG_TYPE_END;
         ptr = (mb_tag_t *)((u8 *)ptr + ((ptr->size + 7) & ~7)))
    {
        switch (ptr->type)
        {
        case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
        {
            mb_tag_basic_meminfo_t *tag = (mb_tag_basic_meminfo_t *)ptr;
            printf("memory lower = %uKB, upper = %uKB\n", tag->mem_lower, tag->mem_upper);
            break;
        }
        case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
        {
            mb_tag_string_t *tag = (mb_tag_string_t *)ptr;
            printf("bootloader = '%s'\n", tag->string);
            break;
        }
        case MULTIBOOT_TAG_TYPE_BOOTDEV:
        {
            mb_tag_bootdev_t *tag = (mb_tag_bootdev_t *)ptr;
            printf("boot device %p,%u,%u\n", (void *)tag->biosdev, tag->part, tag->slice);
            break;
        }
        case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
        {
            mb_tag_framebuffer_t *tag = (mb_tag_framebuffer_t *)ptr;
            printf("framebuffer %p, %ux%ux%u\n", (void *)tag->framebuffer_addr, tag->framebuffer_width, tag->framebuffer_height, tag->framebuffer_bpp);
            break;
        }
        case MULTIBOOT_TAG_TYPE_MMAP:
        {
            mb_tag_mmap_t *tag = (mb_tag_mmap_t *)ptr;

            const MemoryMap mmap(tag->entries, (mb_mmap_entry_t *)((u8 *)tag + tag->size), tag->entry_size);

            printf("memory map:\n");

            for (auto &entry : mmap)
            {
                const char *type_string = "none";
                switch (entry.type)
                {
                case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
                    type_string = "acpi";
                    break;
                case MULTIBOOT_MEMORY_AVAILABLE:
                    type_string = "free";
                    break;
                case MULTIBOOT_MEMORY_BADRAM:
                    type_string = "bad";
                    break;
                case MULTIBOOT_MEMORY_NVS:
                    type_string = "nvs";
                    break;
                case MULTIBOOT_MEMORY_RESERVED:
                    type_string = "reserved";
                    break;
                }
                printf(" base = %p, length = %8uKB, type = %s\n", (void *)entry.base_addr, (u32)(entry.length / 1024), type_string);
            }

            u64 size = Memory_GetSize(mmap);
            printf("memory size = %uKB\n", (u32)(size / 1024));

            break;
        }
        case MULTIBOOT_TAG_TYPE_MODULE:
        {
            mb_tag_module_t *tag = (mb_tag_module_t *)ptr;
            printf("module start = %#x, end = %#x, string = %s\n", tag->mod_start, tag->mod_end, tag->string);
            break;
        }
        case MULTIBOOT_TAG_TYPE_NETWORK:
        {
            mb_tag_network_t *tag = (mb_tag_network_t *)ptr;
            printf("dhcp ack = %s\n", tag->dhcpack);
            break;
        }
        }
    }

    printf("free:     %uKB\n", (u32)(alloc.GetFree() / 1024));
    printf("used:     %uKB\n", (u32)(alloc.GetUsed() / 1024));
    printf("reserved: %uKB\n", (u32)(alloc.GetReserved() / 1024));
}
