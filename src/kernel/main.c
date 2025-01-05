#include <scriptos/framebuffer.h>
#include <scriptos/io.h>
#include <scriptos/multiboot2.h>
#include <scriptos/types.h>

static u32 posx;
static u32 posy;
static Framebuffer_t fb;

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

void print(const char *string)
{
    for (char *p = (char *)string; *p; ++p)
        putchar(*p);
}

void itoa(char *buf, i32 base, i32 d)
{
    char *p = buf;
    char *p1, *p2;
    u32 ud = d;
    i32 divisor = 10;

    if (base == 'd' && d < 0)
    {
        *p++ = '-';
        buf++;
        ud = -d;
    }
    else if (base == 'x')
        divisor = 16;

    do
    {
        i32 remainder = ud % divisor;
        *p++ = (remainder < 10)
                   ? (remainder + '0')
                   : (remainder + 'a' - 10);
    } while (ud /= divisor);

    *p = 0;

    p1 = buf;
    p2 = p - 1;
    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
}

void printf(const char *format, ...)
{
    char **arg = (char **)&format;
    i32 c;
    char buf[20];

    arg++;

    while ((c = *format++) != 0)
    {
        if (c != '%')
            putchar(c);
        else
        {
            char *p, *p2;
            i32 pad0 = 0, pad = 0;

            c = *format++;
            if (c == '0')
            {
                pad0 = 1;
                c = *format++;
            }

            if (c >= '0' && c <= '9')
            {
                pad = c - '0';
                c = *format++;
            }

            switch (c)
            {
            case 'd':
            case 'u':
            case 'x':
                itoa(buf, c, *((i32 *)arg++));
                p = buf;
                goto string;
                break;

            case 's':
                p = *arg++;
                if (!p)
                    p = "(null)";

            string:
                for (p2 = p; *p2; p2++)
                    ;
                for (; p2 < p + pad; p2++)
                    putchar(pad0 ? '0' : ' ');
                while (*p)
                    putchar(*p++);
                break;

            default:
                putchar(*((i32 *)arg++));
                break;
            }
        }
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
            printf("boot device 0x%x,%u,%u\n", tag->biosdev, tag->part, tag->slice);
            break;
        }
        case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
        {
            mb_tag_framebuffer_t *tag = (mb_tag_framebuffer_t *)ptr;

            Framebuffer_Setup(&fb, (void *)tag->framebuffer_addr, tag->framebuffer_width, tag->framebuffer_height, tag->framebuffer_pitch, tag->framebuffer_bpp);
            clear_color();

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
                printf(
                    " base = 0x%x%x, length = 0x%x%x, type = %s\n",
                    (u32)(entry->base_addr >> 32),
                    (u32)(entry->base_addr & 0xffffffff),
                    (u32)(entry->length >> 32),
                    (u32)(entry->length & 0xffffffff),
                    type_string);
            }

            break;
        }
        case MULTIBOOT_TAG_TYPE_MODULE:
        {
            mb_tag_module_t *tag = (mb_tag_module_t *)ptr;
            (void)tag->mod_start;
            (void)tag->mod_end;
            (void)tag->string;
            break;
        }
        case MULTIBOOT_TAG_TYPE_NETWORK:
        {
            mb_tag_network_t *tag = (mb_tag_network_t *)ptr;
            printf("DHCP ACK = %s\n", tag->dhcpack);
            break;
        }
        }
    }
}
