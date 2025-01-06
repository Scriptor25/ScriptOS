#include <scriptos/font.hpp>
#include <scriptos/framebuffer.hpp>
#include <scriptos/info.hpp>
#include <scriptos/io.hpp>
#include <scriptos/memory.hpp>
#include <scriptos/multiboot2.hpp>
#include <scriptos/print.hpp>
#include <scriptos/types.hpp>

static u32 posx;
static u32 posy;
static framebuffer_t fb, bb;

void reset()
{
    posx = posy = 0;
    Framebuffer_Clear(&bb, 0);
}

void draw_char(int c, u32 x, u32 y, u32 color)
{
    auto bmp = BitmapFont_GetChar(c);
    if (!bmp)
        return;
    for (u8 j = 0; j < 8; ++j)
        for (u8 i = 0; i < 8; ++i)
            if (BitmapFont_GetBit(bmp, i, j))
                Framebuffer_Write(&bb, x + i, y + j, color);
}

void draw_test(int offset)
{
    for (u32 j = 0; j < bb.Height; ++j)
        for (u32 i = 0; i < bb.Width; ++i)
        {
            f32 fr = (f32)(((i + offset) * 8) % bb.Width) / (f32)(bb.Width - 1);
            f32 fg = (f32)(((j + offset) * 8) % bb.Height) / (f32)(bb.Height - 1);

            u32 ur = (u32)(fr * 255.999f);
            u32 ug = (u32)(fg * 255.999f);

            u32 color = (ur & 0xff) << 16 | (ug & 0xff) << 8;
            Framebuffer_Write(&bb, i, j, color);
        }
}

void swap_buffers()
{
    Framebuffer_Blit(&fb, &bb);
}

void putchar(int c)
{
    u32 rows = (bb.BPP == 2) ? bb.Height : (bb.Height / 8);
    u32 cols = (bb.BPP == 2) ? bb.Width : (bb.Width / 8);

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

    if (bb.BPP == 2)
        Framebuffer_Write(&bb, posx, posy, 7 << 8 | (c & 0xff));
    else if (bb.BPP == 3 || bb.BPP == 4)
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

        Framebuffer_Setup(&fb, (u8 *)fb_addr, fb_width, fb_height, fb_pitch, fb_bpp);
        Framebuffer_Setup(&bb, (u8 *)KERNEL_END, fb_width, fb_height, fb_pitch, fb_bpp);
        reset();

        draw_test(0);
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
            printf("boot device %#x,%u,%u\n", tag->biosdev, tag->part, tag->slice);
            break;
        }
        case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
        {
            mb_tag_framebuffer_t *tag = (mb_tag_framebuffer_t *)ptr;
            (void)tag;
            break;
        }
        case MULTIBOOT_TAG_TYPE_MMAP:
        {
            mb_tag_mmap_t *tag = (mb_tag_mmap_t *)ptr;

            printf("memory map:\n");

            for (mb_mmap_entry_t *entry = tag->entries;
                 (u8 *)entry < (u8 *)tag + tag->size;
                 entry = (mb_mmap_entry_t *)((u8 *)entry + tag->entry_size))
            {
                const char *type_string = "none";
                switch (entry->type)
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
                printf(" base = %p, length = %8uKB, type = %s\n", entry->base_addr, (u32)(entry->length / 1024), type_string);
            }

            u64 size = Memory_GetSize((mmap_t){
                tag->entries,
                (mb_mmap_entry_t *)((u8 *)tag + tag->size),
                tag->entry_size,
            });
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

    swap_buffers();
}
