#include <scriptos/bitmap.hpp>
#include <scriptos/font.hpp>
#include <scriptos/framebuffer.hpp>
#include <scriptos/gdt.hpp>
#include <scriptos/info.hpp>
#include <scriptos/io.hpp>
#include <scriptos/memory.hpp>
#include <scriptos/multiboot2.hpp>
#include <scriptos/paging.hpp>
#include <scriptos/pfa.hpp>
#include <scriptos/print.hpp>
#include <scriptos/types.hpp>
#include <scriptos/util.hpp>

#define KiB(BYTES) (BYTES / 1024)
#define MiB(BYTES) (BYTES / (1024 * 1024))
#define GiB(BYTES) (BYTES / (1024 * 1024 * 1024))

static u32 posx = 0;
static u32 posy = 0;
static Framebuffer front_buffer;

static void reset()
{
    posx = posy = 0;
    front_buffer.Clear(0x00000000);
}

static void draw_char(int c, u32 x, u32 y, u32 color)
{
    auto bmp = Font_GetChar(c);
    if (!bmp)
        return;
    for (u8 j = 0; j < 8; ++j)
        for (u8 i = 0; i < 8; ++i)
            if (Font_GetBit(bmp, i, j))
                front_buffer.Write(x + i, y + j, color);
}

static void draw_test(int offset, int scale)
{
    for (u32 j = 0; j < front_buffer.GetHeight(); ++j)
        for (u32 i = 0; i < front_buffer.GetWidth(); ++i)
        {
            auto fr = (f32)(((i + offset) * scale) % front_buffer.GetWidth()) / (f32)(front_buffer.GetWidth() - 1);
            auto fg = (f32)(((j + offset) * scale) % front_buffer.GetHeight()) / (f32)(front_buffer.GetHeight() - 1);

            auto ur = (u32)(fr * 255.999f);
            auto ug = (u32)(fg * 255.999f);

            auto color = (ur & 0xff) << 16 | (ug & 0xff) << 8;
            front_buffer.Write(i, j, color);
        }
}

void putchar(int c)
{
    auto rows = (front_buffer.GetBytePerPixel() == 2) ? front_buffer.GetHeight() : (front_buffer.GetHeight() / 8);
    auto cols = (front_buffer.GetBytePerPixel() == 2) ? front_buffer.GetWidth() : (front_buffer.GetWidth() / 8);

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

    if (front_buffer.GetBytePerPixel() == 2)
        front_buffer.Write(posx, posy, 7 << 8 | (c & 0xff));
    else if (front_buffer.GetBytePerPixel() == 3 || front_buffer.GetBytePerPixel() == 4)
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
    if ((magic != MULTIBOOT2_BOOTLOADER_MAGIC) || ((uptr)info & 7))
        return;

    PageTableManager mgr(PageDirectory);

    {
        auto &tag = info->GetTag<multiboot_tag_mmap>(MULTIBOOT_TAG_TYPE_MMAP);
        const MemoryMap mmap(tag.entries, (multiboot_mmap_entry *)((u8 *)&tag + tag.size), tag.entry_size);
        PageFrameAllocator::Get().Init(mmap);

        Paging_Setup();

        auto &page_map = PageFrameAllocator::Get().GetPageMap();
        mgr.MapPages(page_map.GetBuffer(), page_map.GetBuffer(), page_map.GetSize());

        auto page_count = ceil_div((uptr)KERNEL_END - (uptr)KERNEL_START, PAGE_SIZE);
        PageFrameAllocator::Get().LockPages(KERNEL_START, page_count);
        mgr.MapPages(KERNEL_START, KERNEL_START, page_count);
    }

    {
        auto &tag = info->GetTag<multiboot_tag_framebuffer>(MULTIBOOT_TAG_TYPE_FRAMEBUFFER);

        auto addr = tag.framebuffer_addr;
        auto width = tag.framebuffer_width;
        auto height = tag.framebuffer_height;
        auto pitch = tag.framebuffer_pitch;
        auto bpp = tag.framebuffer_bpp;

        front_buffer.Init((u8 *)addr, width, height, pitch, bpp);
        auto page_count = ceil_div(pitch * height, PAGE_SIZE);
        PageFrameAllocator::Get().LockPages((void *)addr, page_count);
        mgr.MapPages((void *)addr, (void *)addr, page_count);

        reset();
        draw_test(0, 1);
    }

    for (auto &entry : *info)
    {
        switch (entry.type)
        {
        case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
        {
            auto &tag = *(const multiboot_tag_basic_meminfo *)&entry;
            printf("memory lower = %uKiB, upper = %uKiB\n", tag.mem_lower, tag.mem_upper);
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
                printf(" base = %p, length = %8uKB, type = %s\n", (void *)entry.base_addr, (u32)KiB(entry.length), type_string);
            }

            auto size = Memory_GetSize(mmap);
            printf("memory size = %uKiB\n", KiB(size));

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
        case MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR:
        {
            auto &tag = *(const multiboot_tag_load_base_addr *)&entry;
            printf("base address = %p\n", (void *)tag.load_base_addr);
            break;
        }
        case MULTIBOOT_TAG_TYPE_ELF_SECTIONS:
        {
            auto &tag = *(const multiboot_tag_elf_sections *)&entry;
            printf("elf sections = %p, entry size = %u, num = %u, shndx = %u\n",
                   tag.sections,
                   tag.entsize,
                   tag.num,
                   tag.shndx);
            break;
        }
        case MULTIBOOT_TAG_TYPE_ACPI_OLD:
        {
            auto &tag = *(multiboot_tag_old_acpi *)&entry;
            printf("old acpi = %p\n", tag.rsdp);
            break;
        }
        case MULTIBOOT_TAG_TYPE_CMDLINE:
        {
            auto &tag = *(multiboot_tag_string *)&entry;
            printf("cmdline = '%s'\n", tag.string);
            break;
        }
        case MULTIBOOT_TAG_TYPE_APM:
        {
            auto &tag = *(multiboot_tag_apm *)&entry;
            printf("apm version = %#x, flags = %016b, offset = %u, cseg = (%p, %uKiB), cseg 16 = (%p, %uKiB), dseg = (%p, %uKiB)\n",
                   tag.version,
                   tag.flags,
                   tag.offset,
                   tag.cseg,
                   KiB(tag.cseg_len),
                   tag.cseg_16,
                   KiB(tag.cseg_16_len),
                   tag.dseg,
                   KiB(tag.dseg_len));
            break;
        }
        case MULTIBOOT_TAG_TYPE_VBE:
        {
            auto &tag = *(multiboot_tag_vbe *)&entry;
            printf("vbe mode = %u, seg = 0x%x, off = 0x%x, len = %u, ctrl info = '%.512s', mode info = '%.256s'\n",
                   tag.vbe_mode,
                   tag.vbe_interface_seg,
                   tag.vbe_interface_off,
                   tag.vbe_interface_len,
                   tag.vbe_control_info,
                   tag.vbe_mode_info);
            break;
        }
        default:
            printf("tag %u\n", entry.type);
            break;
        }
    }

    printf("free:     %uKiB\n", KiB(PageFrameAllocator::Get().GetFree()));
    printf("used:     %uKiB\n", KiB(PageFrameAllocator::Get().GetUsed()));
    printf("reserved: %uKiB\n", KiB(PageFrameAllocator::Get().GetReserved()));

    auto &page_map = PageFrameAllocator::Get().GetPageMap();
    printf("page map size = %u\n", page_map.GetSize());
    for (auto [byte_index_, bit_index_, active_] : page_map)
        front_buffer.Write(posx * 8 + byte_index_, posy * 8 + bit_index_, active_ ? 0xffffffff : 0x00000000);
    posy++;
}
