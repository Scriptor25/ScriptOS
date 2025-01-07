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

static void reset()
{
    posx = posy = 0;
    fb.Clear(0);
}

static void draw_char(int c, u32 x, u32 y, u32 color)
{
    auto bmp = Font_GetChar(c);
    if (!bmp)
        return;
    for (u8 j = 0; j < 8; ++j)
        for (u8 i = 0; i < 8; ++i)
            if (Font_GetBit(bmp, i, j))
                fb.Write(x + i, y + j, color);
}

static void draw_test(int offset)
{
    for (u32 j = 0; j < fb.GetHeight(); ++j)
        for (u32 i = 0; i < fb.GetWidth(); ++i)
        {
            auto fr = (f32)(((i + offset) * 8) % fb.GetWidth()) / (f32)(fb.GetWidth() - 1);
            auto fg = (f32)(((j + offset) * 8) % fb.GetHeight()) / (f32)(fb.GetHeight() - 1);

            auto ur = (u32)(fr * 255.999f);
            auto ug = (u32)(fg * 255.999f);

            auto color = (ur & 0xff) << 16 | (ug & 0xff) << 8;
            fb.Write(i, j, color);
        }
}

void putchar(int c)
{
    auto rows = (fb.GetBytePerPixel() == 2) ? fb.GetHeight() : (fb.GetHeight() / 8);
    auto cols = (fb.GetBytePerPixel() == 2) ? fb.GetWidth() : (fb.GetWidth() / 8);

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

extern "C" void kernel_main(u32 magic, const MultibootInfo *info)
{
    if ((magic != MULTIBOOT2_BOOTLOADER_MAGIC) || ((u32)info & 7))
        return;

    {
        auto &tag = info->GetTag<multiboot_tag_framebuffer>(MULTIBOOT_TAG_TYPE_FRAMEBUFFER);

        auto fb_addr = tag.framebuffer_addr;
        auto fb_width = tag.framebuffer_width;
        auto fb_height = tag.framebuffer_height;
        auto fb_pitch = tag.framebuffer_pitch;
        auto fb_bpp = tag.framebuffer_bpp;

        fb.Init((u8 *)fb_addr, fb_width, fb_height, fb_pitch, fb_bpp);
        reset();

        draw_test(0);
    }

    PageFrameAllocator alloc;
    {
        auto &tag = info->GetTag<multiboot_tag_mmap>(MULTIBOOT_TAG_TYPE_MMAP);
        const MemoryMap mmap(tag.entries, (multiboot_mmap_entry *)((u8 *)&tag + tag.size), tag.entry_size);
        alloc.Init(mmap);
    }

    for (auto &entry : *info)
    {
        switch (entry.type)
        {
        case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
        {
            auto &tag = *(const multiboot_tag_basic_meminfo *)&entry;
            printf("memory lower = %uKB, upper = %uKB\n", tag.mem_lower, tag.mem_upper);
            break;
        }
        case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
        {
            auto &tag = *(const multiboot_tag_string *)&entry;
            printf("bootloader = '%s'\n", tag.string);
            break;
        }
        case MULTIBOOT_TAG_TYPE_BOOTDEV:
        {
            auto &tag = *(const multiboot_tag_bootdev *)&entry;
            printf("boot device %p,%u,%u\n", (void *)tag.biosdev, tag.part, tag.slice);
            break;
        }
        case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
        {
            auto &tag = *(const multiboot_tag_framebuffer *)&entry;
            printf("framebuffer %p, %ux%ux%u\n", (void *)tag.framebuffer_addr, tag.framebuffer_width, tag.framebuffer_height, tag.framebuffer_bpp);
            break;
        }
        case MULTIBOOT_TAG_TYPE_MMAP:
        {
            auto &tag = *(const multiboot_tag_mmap *)&entry;

            const MemoryMap mmap(tag.entries, (const multiboot_mmap_entry *)((u8 *)&tag + tag.size), tag.entry_size);

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

            auto size = Memory_GetSize(mmap);
            printf("memory size = %uKB\n", (u32)(size / 1024));

            break;
        }
        case MULTIBOOT_TAG_TYPE_MODULE:
        {
            auto &tag = *(const multiboot_tag_module *)&entry;
            printf("module start = %#x, end = %#x, string = %s\n", tag.mod_start, tag.mod_end, tag.string);
            break;
        }
        case MULTIBOOT_TAG_TYPE_NETWORK:
        {
            auto &tag = *(const multiboot_tag_network *)&entry;
            printf("dhcp ack = %s\n", tag.dhcpack);
            break;
        }
        }
    }

    printf("free:     %uKB\n", (u32)(alloc.GetFree() / 1024));
    printf("used:     %uKB\n", (u32)(alloc.GetUsed() / 1024));
    printf("reserved: %uKB\n", (u32)(alloc.GetReserved() / 1024));
}
