#include <scriptos/gdt.hpp>
#include <scriptos/graphics.hpp>
#include <scriptos/idt.hpp>
#include <scriptos/info.hpp>
#include <scriptos/io.hpp>
#include <scriptos/mb_info.hpp>
#include <scriptos/pfa.hpp>
#include <scriptos/pic.hpp>
#include <scriptos/print.hpp>
#include <scriptos/ptm.hpp>
#include <scriptos/types.hpp>
#include <scriptos/util.hpp>

static PageTableManager ptm;

static void setup_memory(const MultibootInfo &info)
{
    auto &pfa = PageFrameAllocator::Get();
    auto mmap = info.GetMMap();

    pfa.Init(mmap);
    pfa.LockPages(KERNEL_START, ceil_div((uptr)KERNEL_END - (uptr)KERNEL_START, PAGE_SIZE));

    ptm = PageTableManager((PageDirectoryEntry *)pfa.RequestEmptyPage());
    auto page_count = ceil_div(mmap.Size(), PAGE_SIZE);
    ptm.MapPages(nullptr, nullptr, page_count);
    ptm.SetupPaging();
}

static void setup_graphics(const MultibootInfo &info)
{
    auto &graphics = Graphics::Get();
    auto &pfa = PageFrameAllocator::Get();

    auto &tag = *(multiboot_tag_framebuffer *)info[MULTIBOOT_TAG_TYPE_FRAMEBUFFER];
    auto fb_addr = tag.framebuffer_addr_lo;
    auto width = tag.framebuffer_width;
    auto height = tag.framebuffer_height;
    auto pitch = tag.framebuffer_pitch;
    auto bpp = tag.framebuffer_bpp;

    auto page_count = ceil_div(pitch * height, PAGE_SIZE);
    pfa.LockPages((void *)fb_addr, page_count);
    ptm.MapPages((void *)fb_addr, (void *)fb_addr, page_count);

    auto bb_addr = pfa.RequestPages(page_count);

    graphics.Init((u8 *)fb_addr, (u8 *)bb_addr, width, height, pitch, bpp);

    graphics.Clear(0xff121212);
    graphics.Reset();
    graphics.SetCharColor(0xfffefefe);
}

static void draw_memory_diagram()
{
    auto &graphics = Graphics::Get();
    auto &[px, py] = graphics.Pos();

    auto &pfa = PageFrameAllocator::Get();
    auto &page_map = pfa.PageMap();

    auto w = graphics.Width();

    for (auto [byte_, bit_, set_] : page_map)
    {
        auto x = px + byte_;
        auto y = py + bit_;
        auto color = set_ ? 0xffff0000 : 0xff00ff00;

        y += CHAR_H * (x / w);
        x %= w;

        graphics.DrawPixel(x, y, color);
    }

    auto s = page_map.Size();
    py += CHAR_H * ceil_div(s, w);
}

static void enable_keyboard_interrupt()
{
    asm volatile("cli");

    outb(PIC1_DATA, 0b11111101);
    io_wait();
    outb(PIC2_DATA, 0b11111111);
    io_wait();

    asm volatile("sti");
}

extern "C" void kernel_main(u32 magic, const MultibootInfo &info)
{
    if ((magic != MULTIBOOT2_BOOTLOADER_MAGIC) || ((uptr)&info & 7))
        return;

    setup_memory(info);
    setup_graphics(info);

    InitGDT();
    InitIDT();
    RemapPIC();

    auto mmap = info.GetMMap();
    for (auto &entry : mmap)
    {
        cstr type = nullptr;
        switch (entry.type)
        {
        case MULTIBOOT_MEMORY_AVAILABLE:
            type = "available";
            break;
        case MULTIBOOT_MEMORY_RESERVED:
            type = "reserved";
            break;
        case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
            type = "acpi reclaimable";
            break;
        case MULTIBOOT_MEMORY_NVS:
            type = "nvs";
            break;
        case MULTIBOOT_MEMORY_BADRAM:
            type = "badram";
            break;
        }

        printf("base = %p%p, length = %#08x%08x, type = '%s'\n", entry.base_addr_hi, entry.base_addr_lo, entry.length_hi, entry.length_lo, type);
    }

    enable_keyboard_interrupt();

    auto address = PageFrameAllocator::Get().RequestPage();
    auto test = (u32 *)0xC0000000;

    ptm.MapPage(test, address);

    *test = 123;
    printf("[%p] = %u, [%p] = %u\n", test, *test, address, *(u32 *)address);

    for (;;)
        Graphics::Get().SwapBuffers();
}
