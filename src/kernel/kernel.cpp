#include <scriptos/assert.hpp>
#include <scriptos/gdt.hpp>
#include <scriptos/graphics.hpp>
#include <scriptos/idt.hpp>
#include <scriptos/info.hpp>
#include <scriptos/io.hpp>
#include <scriptos/mb_info.hpp>
#include <scriptos/memory.hpp>
#include <scriptos/pfa.hpp>
#include <scriptos/pic.hpp>
#include <scriptos/print.hpp>
#include <scriptos/ps2.hpp>
#include <scriptos/ptm.hpp>
#include <scriptos/types.hpp>
#include <scriptos/util.hpp>

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

    auto bb_addr = pfa.RequestPage();
    auto size = page_count * PAGE_SIZE;

    for (usize i = PAGE_SIZE; i < size; i += PAGE_SIZE)
    {
        auto address = pfa.RequestPage();
        ptm.MapPage((void *)((uptr)bb_addr + i), address);
    }

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

    auto test1 = (char *)malloc(13);
    test1[0] = 'H';
    test1[1] = 'e';
    test1[2] = 'l';
    test1[3] = 'l';
    test1[4] = 'o';
    test1[5] = ' ';
    test1[6] = 'W';
    test1[7] = 'o';
    test1[8] = 'r';
    test1[9] = 'l';
    test1[10] = 'd';
    test1[11] = '!';
    test1[12] = '\x00';

    auto test2 = (u32 *)malloc(20);
    test2[0] = 0x123;
    test2[1] = 0x456;
    test2[2] = 0x789;
    test2[3] = 0xabc;
    test2[4] = 0xdef;

    printf("%p: %s\n", test1, test1);
    free(test1);

    printf("%p: %x, %x, %x, %x, %x\n", test2, test2[0], test2[1], test2[2], test2[3], test2[4]);
    free(test2);

    auto test3 = (char *)malloc(120);
    test3[0] = 'T';
    test3[1] = 'h';
    test3[2] = 'i';
    test3[3] = 's';
    test3[4] = ' ';
    test3[5] = 'i';
    test3[6] = 's';
    test3[7] = ' ';
    test3[8] = 'a';
    test3[9] = ' ';
    test3[10] = 'T';
    test3[11] = 'e';
    test3[12] = 's';
    test3[13] = 't';
    test3[14] = '!';
    test3[15] = '\x00';

    assert(test1 == test3);
    printf("%p <-> %p: %s\n", test3, test1, test1);
    free(test3);

    for (;;)
        Graphics::GetInstance().SwapBuffers();
}
