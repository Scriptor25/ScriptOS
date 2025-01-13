#include <scriptos/kernel/gdt.hpp>
#include <scriptos/kernel/graphics.hpp>
#include <scriptos/kernel/idt.hpp>
#include <scriptos/kernel/info.hpp>
#include <scriptos/kernel/mb_info.hpp>
#include <scriptos/std/memory.hpp>
#include <scriptos/kernel/pfa.hpp>
#include <scriptos/kernel/pic.hpp>
#include <scriptos/std/print.hpp>
#include <scriptos/kernel/ps2.hpp>
#include <scriptos/kernel/ptm.hpp>
#include <scriptos/std/types.hpp>
#include <scriptos/std/util.hpp>

static void setup_memory(const MultibootInfo &info)
{
    auto &pfa = PageFrameAllocator::GetInstance();
    auto &ptm = PageTableManager::GetKernelInstance();

    auto mmap = info.GetMMap();

    pfa.Init(mmap);
    pfa.LockPages(KERNEL_START, ceil_div((uptr)KERNEL_END - (uptr)KERNEL_START, PAGE_SIZE));

    ptm.Init((PageDirectoryEntry *)pfa.RequestEmptyPage());
    ptm.MapPages(nullptr, nullptr, ceil_div(mmap.Size(), PAGE_SIZE));
    ptm.SetupPaging();
}

static void setup_graphics(const MultibootInfo &info)
{
    auto &pfa = PageFrameAllocator::GetInstance();
    auto &ptm = PageTableManager::GetKernelInstance();
    auto &graphics = Graphics::GetInstance();

    auto &tag = *(multiboot_tag_framebuffer *)info[MULTIBOOT_TAG_TYPE_FRAMEBUFFER];
    auto fb_addr = tag.framebuffer_addr_lo;
    auto width = tag.framebuffer_width;
    auto height = tag.framebuffer_height;
    auto pitch = tag.framebuffer_pitch;
    auto bpp = tag.framebuffer_bpp;

    auto page_count = ceil_div(pitch * height, PAGE_SIZE);
    pfa.LockPages((void *)fb_addr, page_count);
    ptm.MapPages((void *)fb_addr, (void *)fb_addr, page_count);

    auto bb_addr = malloc(pitch * height);
    graphics.Init((u8 *)fb_addr, (u8 *)bb_addr, width, height, pitch, bpp);

    graphics.Clear(0xff121212);
    graphics.Reset();
    graphics.SetCharColor(0xfffefefe);
}

static void draw_memory_diagram()
{
    auto &graphics = Graphics::GetInstance();
    auto &[px, py] = graphics.Pos();

    auto &pfa = PageFrameAllocator::GetInstance();
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

    PS2_Enable_Keyboard_Interrupt();

    for (;;)
        Graphics::GetInstance().SwapBuffers();
}
