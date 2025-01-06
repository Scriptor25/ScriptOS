#include <scriptos/framebuffer.h>
#include <scriptos/io.h>
#include <scriptos/multiboot2.h>
#include <scriptos/print.h>
#include <scriptos/types.h>

static u32 posx;
static u32 posy;
static framebuffer_t fb;

void reset()
{
    posx = posy = 0;
    Framebuffer_Clear(&fb, 0);
}

void clear_color()
{
    for (u32 j = 0; j < fb.Height; ++j)
        for (u32 i = 0; i < fb.Width; ++i)
        {
            f32 fr = (f32)i / (f32)(fb.Width - 1);
            f32 fg = (f32)j / (f32)(fb.Height - 1);

            u32 ur = (u32)(fr * 255.999f);
            u32 ug = (u32)(fg * 255.999f);

            u32 color = (ur & 0xff) << 16 | (ug & 0xff) << 8;
            Framebuffer_Write(&fb, i, j, color);
        }
}

void putchar(i32 c)
{
    if (c < 0x20)
    {
        switch (c)
        {
        case '\n':
            posx = 0;
            if (++posy >= fb.Height)
                posy = 0;
            break;
        case '\r':
            posx = 0;
            break;
        }
        return;
    }

    Framebuffer_Write(&fb, posx, posy, 7 << 8 | (c & 0xff));

    if (++posx >= fb.Width)
    {
        posx = 0;
        if (++posy >= fb.Height)
            posy = 0;
    }
}

void main(u32 magic, u32 addr)
{
    if ((magic != MULTIBOOT2_BOOTLOADER_MAGIC) || (addr & 7))
        return;

    Framebuffer_Setup(&fb, (void *)0xB8000, 80, 25, 160, 16);
    reset();

    for (mb_tag_t *ptr = (mb_tag_t *)addr + 1;
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

            Framebuffer_Setup(&fb, (void *)(u32)tag->framebuffer_addr, tag->framebuffer_width, tag->framebuffer_height, tag->framebuffer_pitch, tag->framebuffer_bpp);
            // clear_color();

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
                printf(" base = %p, length = %p, type = %s\n", entry->base_addr, entry->length, type_string);
            }

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
}
